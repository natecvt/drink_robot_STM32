#ifndef MENU_ELEMENTS_H
#define MENU_ELEMENTS_H


#include "hal_defines.h"
#include "fonts_local.h"
#include "menu.h"

#define FONT       &Font_7x10
#define COLOR      SH1106_COLOR_WHITE

char __list_type_menu_SH1106_print(Menu *cur_menu, uint8_t menu_pos);
void __progress_bar_menu_element_SH1106_print(uint8_t percent, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#endif /** MENU_ELEMENTS_H */