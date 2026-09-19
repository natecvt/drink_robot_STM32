#include "button.h"

volatile uint8_t signal_flags = 0;

Button_Err_t button_init(uint8_t button_index, uint16_t gpio)
{
    if (button_index > BUTTON_LEN - 1) return BUTTON_UNKNOWN;

    #ifndef TESTING
    button_HAL_init(&buttons_HAL[button_index], gpio);
    #endif

    return BUTTON_OK;
}

Button_Err_t button_get_state(Button_State_t *bs, uint8_t button_index)
{
    if (button_index > BUTTON_LEN - 1) return BUTTON_UNKNOWN;

    #ifndef TESTING
    if (buttons_HAL[button_index].gpio_bus == 0) return BUTTON_UNINITIALIZED;
    (*bs) = buttons_HAL[button_index].state;
    #endif

    return BUTTON_OK;
}