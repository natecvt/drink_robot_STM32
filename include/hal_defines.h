#ifndef HAL_DEFINES_H
#define HAL_DEFINES_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdint.h>

/** Buttons */

#define BUTTON_ARR_S 5
#define BUTTON_LEFT 0
#define BUTTON_UP 1
#define BUTTON_RIGHT 2
#define BUTTON_DOWN 3
#define BUTTON_SEL 4

#define DEBOUNCE_MS 50
#define BUTTON_TIMER TIM3

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

uint8_t button_HAL_init(Button_HAL_t button, GPIO_TypeDef* gpio_bus, uint16_t gpio);
uint8_t buttons_HAL_isr(uint16_t gpio);
void buttons_HAL_overflow_handler();

#endif /** HAL_DEFINES_H */