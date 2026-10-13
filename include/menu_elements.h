#ifndef MENU_ELEMENTS_H
#define MENU_ELEMENTS_H

#include "hal_defines.h"
#include "fonts_local.h"
#include "menu.h"
#include "drinks.h"
#include "stdio.h"

#define FONT       &Font_7x10
#define COLOR      SH1106_COLOR_WHITE

#define CAL_MENU_PB_Y 56
#define CAL_MENU_PB_H 7
#define CAL_MENU_PB_W 100

#define CAL_MENU_TR_H 10
#define CAL_MENU_TR_W 7

char __list_type_menu_SH1106_print(uint8_t menu_pos);
void __progress_bar_menu_element_SH1106(uint8_t percent, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
char __string_menu_element_SH1106(char *s, uint16_t x, uint16_t y);
char __calibration_type_menu_SH1106_print(Ingredient *ig, uint8_t ig_index);


#endif /** MENU_ELEMENTS_H */