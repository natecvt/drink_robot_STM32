#include "hal_defines.h"
#include "button.h"

Stepper_t steppers[STEPPER_NUM];

Stepper_Err_t stepper_init(uint8_t axis, Stepper_Init_t *stepper, uint16_t steps_per_rotation)
{
    if (axis > 2) return STEPPER_INIT_ERR;
    memcpy(&(steppers[axis]), stepper, S(Stepper_Init_t));

    steppers[axis].spr = steps_per_rotation;
    steppers[axis].states[0] = GPIO_PIN_RESET;
    steppers[axis].states[1] = GPIO_PIN_RESET;
    steppers[axis].stepcount = 0;
    steppers[axis].homed = 0;
    steppers[axis].coord = 0.0;

    /** make sure the driver is not enabled, pull high through a resistor when not in use */
    stepper_set_enable(axis, 0);
    /** start the associated timer */
    HAL_TIM_Base_Start_IT(steppers[axis].timer);
    stepper_set_speed(axis, 0);

    return STEPPER_OK;
}

void stepper_step(uint8_t axis)
{
    static Stepper_t *stepper = {0};
    stepper = &(steppers[axis]);
    if (stepper->stepcount) {
        HAL_GPIO_TogglePin(stepper->stp_bus, stepper->stp_gpio);
        stepper->stepcount--;
    }
    else {
        stepper_set_enable(axis, 0);
        stepper_set_speed(axis, 0);
    }
}

void stepper_set_distance(uint8_t axis, double dist)
{
    static int32_t steps = 0;
    steps = (int32_t)(dist / steppers[axis].ratio);
    
    stepper_set_direction(axis, steps > 0.0);

    steppers[axis].coord += dist; /** intended destination */
    steppers[axis].stepcount = ABS(steps);
}

void stepper_set_speed(uint8_t axis, uint16_t rpm)
{
    static TIM_TypeDef *t;
    t = steppers[axis].timer->Instance;

    /** disable interrupt if speed is zero */
    if (rpm == 0) {
        t->DIER &= ~TIM_DIER_UIE;
        return;
    }
    t->DIER |= TIM_DIER_UIE;

    /** TODO: look into whether this breaks for different timers */
    uint32_t fclk = HAL_RCC_GetPCLK2Freq(); // current clock frequency
    uint32_t ctpp = fclk / (steppers[axis].spr * rpm) * 60; // current clock ticks per pulse
    uint16_t m = ctpp >> 16; // divide by max CNT = 65536

    t->ARR = ctpp / (m+1); // new ARR is nearest value below 0xFFFF, highest definition clock that fits ctpp
    t->PSC = m; // new prescale is m, since m+1 scales ARR below 0xFFFF
}

void stepper_set_direction(uint8_t axis, GPIO_PinState dir)
{
    HAL_GPIO_WritePin(steppers[axis].dir_bus, steppers[axis].dir_gpio, dir);
    steppers[axis].states[1] = dir;
}

void stepper_set_enable(uint8_t axis, GPIO_PinState en)
{
    /** most stepper drivers invert enable logic */
    HAL_GPIO_WritePin(steppers[axis].en_bus, steppers[axis].en_gpio, !en);
    steppers[axis].states[0] = en;
}

void stepper_update_state(uint8_t axis, double dist, uint16_t rpm, GPIO_PinState en)
{
    stepper_set_enable(axis, 0);
    stepper_set_speed(axis, 0);
    stepper_set_distance(axis, dist);
    stepper_set_enable(axis, en);
    stepper_set_speed(axis, rpm);
}

void stepper_block_until_complete(uint8_t axis)
{
    static Stepper_t *stepper = {0};
    stepper = &(steppers[axis]);
    while (stepper->stepcount);
}

double stepper_report_current_coord(uint8_t axis)
{
    /** planned destination - distance to go */
    return steppers[axis].coord - 
        (-1 * steppers[axis].states[1] + 1 * !steppers[axis].states[1]) * 
        (double)(steppers[axis].stepcount) * steppers[axis].ratio;
}

void stepper_home(uint8_t axis)
{
    Button_State_t bs;
    static Stepper_t *stepper = {0};
    stepper = &(steppers[axis]);

    /** homing procedure will not work - no button attached */
    if (!stepper->limit_index) return;

    /** wait until limit button is pressed */
    while (bs != BUTTON_HIGH) {
        stepper_update_state(axis, -10.0, 10, 1);
        HAL_Delay(1);
        button_get_state(&bs, stepper->limit_index);
    }

    /** un-press the button at a low speed */
    while (bs != BUTTON_LOW) {
        stepper_update_state(axis, 2.0, 2, 1);
        HAL_Delay(1);
        button_get_state(&bs, stepper->limit_index);
    }

    stepper_set_enable(axis, 0);

    /** reset the created signal */
    BIT_RESET(signal_flags, stepper->limit_index);

    steppers[axis].homed = 1;
    steppers[axis].coord = 0.0;
}
