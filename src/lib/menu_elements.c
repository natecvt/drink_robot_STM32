#include "menu_elements.h"

char __list_type_menu_SH1106_print(uint8_t menu_pos)
{
    uint8_t bottom = min(cur_menu->count, ROWS),
        ret = 0;

    SH1106_Clear();
    ret |= __string_menu_element_SH1106(cur_menu->title, 0, 0);
    SH1106_DrawLine(0, (FONT)->FontHeight-1, SH1106_WIDTH-1, (FONT)->FontHeight-1, COLOR);

    for (uint8_t i = 0; i<bottom; i++) {
        ret |= __string_menu_element_SH1106(cur_menu->items[i].text, 0, (FONT)->FontHeight * (i+1));
    }

    uint16_t y = (menu_pos + 1) * (FONT)->FontHeight;
    SH1106_DrawRectangle(0, y, SH1106_WIDTH-1, (FONT)->FontHeight-2, COLOR);

    SH1106_UpdateScreen();
    return ret;
}

char __string_menu_element_SH1106(char *s, uint16_t x, uint16_t y)
{
    SH1106_GotoXY(x, y);
    return SH1106_Puts(s, FONT, COLOR);
}

void __progress_bar_menu_element_SH1106(uint8_t percent, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (height < 5) return; /** not possible to render, needs at least 5 pixels*/
    if (x + width > SH1106_WIDTH-1) return;
    if (y + height > SH1106_HEIGHT-1) return;

    uint16_t right_edge_filled = (uint16_t)(((float)percent / 100.0f) * (float)(width - 4));

    SH1106_DrawRectangle(x, y, width, height, COLOR);
    SH1106_DrawFilledRectangle(x+2, y+2, right_edge_filled, height-4, COLOR);

}

void __triangle_element_SH1106(uint8_t points_left, uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{
    /** restricted type of left/right-pointing arrow */
    if (points_left) SH1106_DrawTriangle(x1, y1+(h/2), x1+w, y1, x1+w, y1+h, COLOR);
    else SH1106_DrawTriangle(x1+w, y1+(h/2), x1, y1, x1, y1+h, COLOR);
}

char __calibration_type_menu_SH1106_print(Ingredient *ig, uint8_t ig_index)
{
    uint8_t ret = 0;
    SH1106_Clear();

    static double location = 0.0;
    location = stepper_report_current_coord(AXIS_X);

    static uint8_t location_normal = 0;
    location_normal = (uint8_t)(100.0 * location / X_LIMIT);

    /** still display the menu even if the coord is off the side */
    ret |= location_normal > 100;

    __progress_bar_menu_element_SH1106(location_normal, 
        (SH1106_WIDTH - CAL_MENU_PB_W)>>1, 
        CAL_MENU_PB_Y, 
        CAL_MENU_PB_W, 
        CAL_MENU_PB_H);

    ret |= __string_menu_element_SH1106(cur_menu->title, 0, 0);
    SH1106_DrawLine(0, (FONT)->FontHeight-1, SH1106_WIDTH-1, (FONT)->FontHeight-1, COLOR);

    ret |= __string_menu_element_SH1106(ig->name, 0, 2*(FONT)->FontHeight);

    static char s[MAX_NAMEC];
    memset(s, 0, MAX_NAMEC);
    snprintf(s, MAX_NAMEC, "Ingredient #: %d", ig_index);
    ret |= __string_menu_element_SH1106(s, 0, 3*(FONT)->FontHeight);

    /** 
     * make triangle indicating current direction 
     * #TODO: create jogging functionality for the calibration
     */
    if (signal_flags & LEFT) {
        __triangle_element_SH1106(1, 
            1, 
            4*(FONT)->FontHeight,
            CAL_MENU_TR_W,
            CAL_MENU_TR_H);
        
        BIT_RESET_MASK(signal_flags, LEFT);
    }
    if (signal_flags & RIGHT) {
        __triangle_element_SH1106(0, 
            SH1106_WIDTH-CAL_MENU_TR_W-2, 
            4*(FONT)->FontHeight,
            CAL_MENU_TR_W,
            CAL_MENU_TR_H);

        BIT_RESET_MASK(signal_flags, RIGHT);
    }

    memset(s, 0, MAX_NAMEC);
    snprintf(s, MAX_NAMEC, "%4.1f", (float)location);
    ret |= __string_menu_element_SH1106(s, 
        ((MAX_NAMEC - 6) >> 1) * (FONT)->FontWidth, 
        4*(FONT)->FontHeight);

    SH1106_UpdateScreen();
    return ret;
}

