#ifndef HAL_DEFINES_H
#define HAL_DEFINES_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdint.h>

/** --- DEFINES --- */
/** Buttons */

#define DEBOUNCE_MS 50
#define TIMER_ARR_INCR_S 0.0001 // tenth of millisecond
#define TIMER_DIFF (uint16_t)((double)DEBOUNCE_MS / (1000.0 * TIMER_ARR_INCR_S))
#define BUTTON_TIMER TIM3

/** --- DATA --- */
/** Buttons */

enum {
    BUTTON_SEL = 0,
    BUTTON_RIGHT = 1,
    BUTTON_DOWN = 2,
    BUTTON_UP = 3,
    BUTTON_LEFT = 4,
    BUTTON_LEN = 5
};

typedef enum {
    BUTTON_LOW = GPIO_PIN_RESET,
    BUTTON_HIGH = GPIO_PIN_SET,
} Button_State_t;

typedef enum {
    BUTTON_HAL_OK = 0,
    BUTTON_HAL_TIM_NOT_ELAPSED = 1,
    BUTTON_HAL_UNKNOWN = 2,
} Button_HAL_Err_t;

/** #TODO: resolve HAL/native button interdependency */
typedef struct {
    /** gpio pins used by the button */
    GPIO_TypeDef* gpio_bus;
    uint16_t gpio;

    /** timer items */
    uint8_t overflow;
    uint16_t timestamp;
} Button_HAL_t;

extern Button_HAL_t buttons_HAL[];

void button_HAL_timer_init();
void button_HAL_init(Button_HAL_t *button, GPIO_TypeDef* gpio_bus, uint16_t gpio);
uint8_t button_HAL_index_from_gpio(uint16_t gpio);
Button_HAL_Err_t buttons_HAL_isr(Button_State_t *bs, uint8_t button_index);
void buttons_HAL_overflow_handler();

#endif /** HAL_DEFINES_H */
