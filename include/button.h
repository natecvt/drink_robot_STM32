#ifndef BUTTON_H
#define BUTTON_H

/** TESTING ENVIRONMENT SETUP */
#ifdef TESTING
    #include "stdint.h"
    #define BUTTON_LEN 2

    typedef enum {
    BUTTON_LOW = 0,
    BUTTON_HIGH = 1,
    } Button_State_t;

#else
    #include "hal_defines.h"
#endif

typedef enum {
    BUTTON_OK = 0,
    BUTTON_UNINITIALIZED = 1,
    BUTTON_UNKNOWN = 2,
} Button_Err_t;

Button_Err_t button_init(uint8_t button_index, uint16_t gpio);
Button_Err_t button_get_state(Button_State_t *bs, uint8_t button_index);

#endif /** BUTTON_H */