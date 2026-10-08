#include "hal_defines.h"

Stepper_t steppers[STEPPER_NUM];

Stepper_Err_t stepper_init(uint8_t axis, Stepper_Init_t *stepper, uint16_t steps_per_rotation)
{
    if (axis > 2) return STEPPER_INIT_ERR;
    memcpy(&(steppers[axis]), stepper, S(Stepper_Init_t));

    steppers[axis].spr = steps_per_rotation;
    steppers[axis].states[0] = GPIO_PIN_RESET;
    steppers[axis].states[1] = GPIO_PIN_RESET;
    steppers[axis].stepcount = 0;

    /** make sure the driver is not enabled, pull high through a resistor when not in use */
    HAL_GPIO_WritePin(steppers[axis].en_bus, steppers[axis].en_gpio, GPIO_PIN_SET);
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

void stepper_update_state(uint8_t axis, uint32_t steps, uint16_t rpm, GPIO_PinState dir, GPIO_PinState en)
{
    static Stepper_t *stepper = {0};
    stepper = &(steppers[axis]);

    /** temporarily lock stepper isr, don't disable pin because other things might be happening */
    stepper_set_speed(axis, 0);

    stepper->stepcount = steps;
    stepper->states[0] = en;
    stepper->states[1] = dir;

    stepper_set_direction(axis, dir);
    stepper_set_enable(axis, en);
    stepper_set_speed(axis, rpm);
}

void stepper_block_until_complete(uint8_t axis)
{
    static Stepper_t *stepper = {0};
    stepper = &(steppers[axis]);
    while (stepper->stepcount);
}