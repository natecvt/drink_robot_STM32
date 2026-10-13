#ifndef MOTION_PLANNER_H
#define MOTION_PLANNER_H

#include "hal_defines.h"
#include "ringbuf.h"

typedef enum Mot_Plan_Err {
    MP_OK = 0,
    MP_PRO_ERR = 1,
    MP_CON_ERR = 2,

} Mot_Plan_Err_t;

typedef struct Plan {
    double dists[STEPPER_NUM];
    uint16_t speeds[STEPPER_NUM];
} Plan_t;

/** ringbuffer behavior is applied to this array */
Plan_t plans[RB_SIZE];
extern Plan_t mod_plan;

void mplan_init();
Mot_Plan_Err_t mplan_produce_preplan();
Mot_Plan_Err_t mplan_produce_plan(double *dists, uint16_t *speeds);
uint8_t mplan_query_current_plan(Plan_t *pcplan);
Mot_Plan_Err_t mplan_consume_plan();

#endif /** MOTION_PLANNER_H */
