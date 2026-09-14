#include "hal_defines.h"

Button_HAL_t buttons_HAL[BUTTON_LEN] = {};

void button_HAL_timer_init()
{
    /** TODO: make this part timer-agnostic */
    double f = (double)HAL_RCC_GetPCLK2Freq();

    /** valid debounce between 0 and 6.5s, using TIMER_ARR_INC for delta */
    BUTTON_TIMER->ARR = TIMER_DIFF;
    BUTTON_TIMER->PSC = (uint32_t)(TIMER_ARR_INCR_S * f) - 1;

}

void button_HAL_init(Button_HAL_t *button, uint16_t gpio)
{

    button->gpio_bus = BUTTON_GPIO_BUS;
    button->gpio = gpio;

    button->protected = 0;
    button->state = GPIO_PIN_RESET;
}

uint8_t button_HAL_index_from_gpio(uint16_t gpio)
{
    for (uint8_t i = 0; i < BUTTON_LEN; i++) {
        if (buttons_HAL[i].gpio == gpio) return i;
    }

    return BUTTON_LEN;
}

/**
 * outputs state if debounce time has elapsed, for use after button index has been resolved
 */
Button_HAL_Err_t buttons_HAL_isr(uint8_t button_index)
{
    if (button_index > BUTTON_LEN - 1) return BUTTON_HAL_UNKNOWN;

    if (buttons_HAL[button_index].protected) return BUTTON_HAL_PROTECTED;

    buttons_HAL[button_index].protected = 1;
    buttons_HAL[button_index].state = HAL_GPIO_ReadPin(buttons_HAL[button_index].gpio_bus, buttons_HAL[button_index].gpio);

    /** reset the timer counter */
    BUTTON_TIMER->CNT = 0;
    
    return BUTTON_HAL_OK;
}

void buttons_HAL_reset()
{
    for (uint8_t i = 0; i < BUTTON_LEN; i++) {
        if (buttons_HAL[i].protected) {
            buttons_HAL[i].state = HAL_GPIO_ReadPin(buttons_HAL[i].gpio_bus, buttons_HAL[i].gpio);
        }

        buttons_HAL[i].protected = 0;
    }
}
