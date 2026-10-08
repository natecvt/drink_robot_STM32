#include "motion_planner.h"

Plan_t plans[RB_SIZE];
Plan_t mod_plan = {0};

void mplan_init()
{
    memset(plans, 0, S(Plan_t) * RB_SIZE);
    ringbuf_init();
}

void __mplan_assign_plan_to_motors(Plan_t *p)
{
    uint8_t do_enable = 1;
    for (uint8_t i = 0; i < STEPPER_NUM; i++) {
        stepper_update_state(i, p->steps[i], p->speeds[i], p->directions[i], 0);
        do_enable &= (p->steps[i] == 0);
    }

    /** only for all-enable, #TODO: make this more robust */
    stepper_set_enable(AXIS_X, !do_enable);
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

Mot_Plan_Err_t mplan_produce_plan(uint32_t *steps, uint16_t *speeds, uint8_t *dirs)
{
    static Plan_t *cplan = NULL;
    cplan = &(plans[rb_pindex % RB_SIZE]);

    memcpy(cplan->steps, steps, S(uint32_t) * STEPPER_NUM);
    memcpy(cplan->speeds, speeds, S(uint16_t) * STEPPER_NUM);
    memcpy(cplan->directions, dirs, S(uint8_t) * STEPPER_NUM);

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
        cplan->steps[i] = steppers[i].stepcount;
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
    if (ringbuf_consume()) return MP_CON_ERR; /** #TODO: figure out what to do if return happens here */
    
    /** assigns nothing if reaches the end */
    cplan = &(plans[rb_cindex % RB_SIZE]);
    __mplan_assign_plan_to_motors(cplan);

    return MP_OK;
}