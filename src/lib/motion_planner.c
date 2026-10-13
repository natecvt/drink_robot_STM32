#include "motion_planner.h"

Plan_t plans[RB_SIZE];
Plan_t mod_plan = {0};

/** because I don't know if 0.0 is all zero bytes */
double template[STEPPER_NUM] = {0.0, 0.0, 0.0};

void mplan_init()
{
    memset(plans, 0, S(Plan_t) * RB_SIZE);

    for (uint8_t i = 0; i < RB_SIZE; i++) {
        memcpy(plans[i].dists, template, S(template));
    }

    ringbuf_init();
}

void __mplan_assign_plan_to_motors(Plan_t *p)
{
    for (uint8_t i = 0; i < STEPPER_NUM; i++) {
        stepper_update_state(i, p->dists[i], p->speeds[i], 
            p->dists[i] <= SMALL_NUM && p->dists[i] >= -SMALL_NUM);
    }
}

Mot_Plan_Err_t mplan_produce_preplan()
{
    static Plan_t *cplan = NULL;
    cplan = &(plans[rb_pindex % RB_SIZE]);

    memcpy(cplan, &mod_plan, S(Plan_t));

    /** if there are no currently processing plans, assign it */
    if (rb_pindex == rb_cindex) __mplan_assign_plan_to_motors(cplan);

    return ringbuf_produce((void *)cplan);
}

Mot_Plan_Err_t mplan_produce_plan(double *dists, uint16_t *speeds)
{
    static Plan_t *cplan = NULL;
    cplan = &(plans[rb_pindex % RB_SIZE]);

    memcpy(cplan->dists, dists, S(double) * STEPPER_NUM);
    memcpy(cplan->speeds, speeds, S(uint16_t) * STEPPER_NUM);

    /** if there are no currently processing plans, assign it */
    if (rb_pindex == rb_cindex) __mplan_assign_plan_to_motors(cplan);

    return ringbuf_produce((void *)cplan);
}

uint8_t mplan_query_current_plan(Plan_t *pcplan)
{
    static Plan_t *cplan = NULL;
    cplan = &(plans[rb_cindex % RB_SIZE]);
    if (pcplan != NULL) pcplan = cplan; /** variable out if needed */
    
    uint8_t retval = 1;

    for (uint8_t i = 0; i < STEPPER_NUM; i++) {
        cplan->dists[i] = stepper_report_current_coord(i);
        retval &= (steppers[i].stepcount == 0);
    }

    return retval;
}

Mot_Plan_Err_t mplan_consume_plan()
{
    static Plan_t *cplan = NULL;
    
    /** find plan that has reached its end, reset to zero */
    cplan = &(plans[rb_cindex % RB_SIZE]);
    memset(cplan, 0, S(Plan_t));
    memcpy(cplan->dists, template, S(template));
    if (ringbuf_consume()) return MP_CON_ERR; /** #TODO: figure out what to do if return happens here */
    
    /** assigns nothing if reaches the end */
    cplan = &(plans[rb_cindex % RB_SIZE]);
    __mplan_assign_plan_to_motors(cplan);

    return MP_OK;
}
