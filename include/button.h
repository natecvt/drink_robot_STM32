#ifndef BUTTON_H
#define BUTTON_H

#include "hal_defines.h"

typedef enum {
    BUTTON_OK = 0,
    BUTTON_UNKNOWN = 1,
} Button_Err_t;

Button_Err_t button_init(uint8_t button_index, uint16_t gpio);
Button_Err_t button_get_state(Button_State_t *bs, uint8_t button_index);

#endif /** BUTTON_H */