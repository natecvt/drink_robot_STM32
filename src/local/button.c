#include "button.h"

Button_t buttons[BUTTON_LEN] = {};

Button_Err_t button_init(uint8_t button_index, uint16_t gpio)
{
    if (button_index > BUTTON_LEN - 1) return BUTTON_UNKNOWN;

    button_HAL_init(&buttons_HAL[button_index], gpio);

    buttons[button_index].button_HAL = &(buttons_HAL[button_index]);
    buttons[button_index].state = BUTTON_LOW;

    return BUTTON_OK;
}

Button_Err_t button_get_state(Button_State_t *bs, uint8_t button_index)
{
    if (button_index > BUTTON_LEN - 1) return BUTTON_UNKNOWN;
    (*bs) = buttons[button_index].state;

    return BUTTON_OK;
}