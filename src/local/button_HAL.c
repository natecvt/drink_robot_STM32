#include "HAL_defines.h"

void button_timer_init()
{
    /** TODO: make this part timer-agnostic */
    double f = (double)HAL_RCC_GetPCLK2Freq();

    /** valid debounce between 0 and 1s, using TIMER_ARR_INC for delta */
    BUTTON_TIMER->ARR = UINT16_MAX;
    BUTTON_TIMER->PSC = (uint32_t)(TIMER_ARR_INCR_S * f) - 1;

}

void button_HAL_init(Button_HAL_t *button, GPIO_TypeDef *gpio_bus, uint16_t gpio)
{

    button->gpio_bus = gpio_bus;
    button->gpio = gpio;

    button->overflow = 0;
    button->timestamp = BUTTON_TIMER->CNT;
}

uint8_t buttons_HAL_isr(Button_State *bs, uint8_t button_index)
{
    uint16_t cnt = BUTTON_TIMER->CNT;

    Button_HAL_t curr_button = buttons_HAL[button_index];
    /** timer has not been fully elapsed, so return */
    if (curr_button.overflow || cnt < curr_button.timestamp) return BUTTON_HAL_TIM_NOT_ELAPSED;
    /** read state if timer has passed */
    (*bs) = HAL_GPIO_ReadPin(curr_button.gpio_bus, curr_button.gpio);

    /** update the timestamp and overflow for next debounce period */
    curr_button.timestamp = cnt + TIMER_DIFF;
    curr_button.overflow = ((curr_button.timestamp) >> 16) != 0;
    
    return BUTTON_HAL_OK;
}

void buttons_HAL_overflow_handler(uint8_t button_arr_size)
{
    /** reset all buttons with overflow = 1 on timer elapsed cb call*/
    for (uint8_t i = 0; i < button_arr_size; i++) {
        buttons_HAL[i].overflow = 0;
    }
}
