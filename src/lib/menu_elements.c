#include "menu_elements.h"

char __list_type_menu_SH1106_print(Menu *cur_menu, uint8_t menu_pos)
{
    uint8_t bottom = min(cur_menu->count, ROWS),
        ret = 0;

    SH1106_Clear();
    SH1106_GotoXY(0, 0);
    ret |= SH1106_Puts(cur_menu->title, FONT, COLOR);
    SH1106_DrawLine(0, (FONT)->FontHeight-1, SH1106_WIDTH-1, (FONT)->FontHeight-1, COLOR);

    for (uint8_t i = 0; i<bottom; i++) {
        SH1106_GotoXY(0, (FONT)->FontHeight * (i+1));
        ret |= SH1106_Puts(cur_menu->items[i].text, FONT, COLOR);
    }

    uint16_t y = (menu_pos + 1) * (FONT)->FontHeight;
    SH1106_DrawRectangle(0, y, SH1106_WIDTH-1, (FONT)->FontHeight-2, COLOR);

    SH1106_UpdateScreen();
    return ret;
}

void __progress_bar_menu_element_SH1106_print(uint8_t percent, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (height < 5) return; /** not possible to render, needs at least 5 pixels*/
    if (x + width > SH1106_WIDTH) return;
    if (y + height > SH1106_HEIGHT) return;
    SH1106_Clear();

    uint16_t right_edge_filled = x + 2 + (uint16_t)(((float)percent / 100.0f) * (float)(width - 4));

    SH1106_DrawRectangle(x, y, width, height, SH1106_COLOR_WHITE);
    SH1106_DrawFilledRectangle(x+2, y+2, right_edge_filled, height-4, SH1106_COLOR_WHITE);

    SH1106_UpdateScreen();
}