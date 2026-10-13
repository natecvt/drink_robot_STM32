#include "menu.h"
#include "menu_elements.h"

Menu * cur_menu;

/** internal variables */
static uint8_t menu_index = 0, /** index into menu-relevant data */
    menu_pos = 0; /** position from the top, init-ed to zero */
    
void menu_init(uint8_t going_back)
{
    menu_index = 0;
    menu_pos = 0;

    if (going_back) {
        cur_menu = cur_menu->supmenu;
    }
    else {
        cur_menu = cur_menu->items[menu_pos].submenu;
    }
}

void menu_init_entry(Menu *main)
{
    cur_menu = main;
}

Menu_Err_t menu_print()
{
    /** TODO: expand on this
     *  should only update menu when menu is changed
     */
    return __list_type_menu_SH1106_print(menu_pos);
}

Menu_Err_t menu_update_list(uint8_t up)
{
    if (cur_menu->count <= ROWS) return MENU_NO_ACTION;
    /** 
     * WARNING: this does not check the bounds of the underlying list
     * count should've been initialized correctly to avoid errors
     */

    if (up) menu_index--;
    else menu_index++;

    if (cur_menu->item_regime == MENU_ITEMS_PREALLOC) {
        if (up) cur_menu->items--;
        else cur_menu->items++;
    }
    else {
        BIT_SET_MASK(signal_flags, DATA_FETCH);
    }

    return MENU_OK;
}

Menu_Err_t menu_scroll()
{
    if (signal_flags & UP) {
        BIT_RESET_MASK(signal_flags, UP);
        if (menu_index + menu_pos == 0) return MENU_LIST_BOUNDS_ERR;

        if (menu_pos > 0) menu_pos--;
        else return menu_update_list(1); /** if menu_pos is at top, but menu_index isn't */

        return MENU_OK;
    }

    if (signal_flags & DOWN) {
        BIT_RESET_MASK(signal_flags, DOWN);
        if (menu_index + menu_pos == cur_menu->count - 1) return MENU_LIST_BOUNDS_ERR;
        
        if (menu_pos < ROWS - 1) menu_pos++;
        else return menu_update_list(0); /** if menu_pos is bottom, but menu_index isn't */

        return MENU_OK;
    }

    return MENU_NO_ACTION;
}

Menu_Err_t menu_select()
{
    if ((signal_flags & BACK) == BACK) {
        BIT_RESET_MASK(signal_flags, BACK);
        if (cur_menu->supmenu == NULL) return MENU_NULL_ERR;

        menu_init(1);

        return MENU_OK;
    }

    if (signal_flags & SELECT) {

        uint8_t i = menu_index + menu_pos;

        BIT_RESET_MASK(signal_flags, SELECT);
        if (cur_menu->items[i].submenu == NULL) return MENU_NULL_ERR;

        if (cur_menu->items[i].action != NULL) cur_menu->items[i].action();
        menu_init(0);

        return MENU_OK;
    }

    return MENU_NO_ACTION;
}

Menu_Err_t menu_signal() 
{
    Menu_Err_t ret = MENU_OK;

    /** promote ret to the highest (most severe erred output) */
    ret |= menu_select();

    /** only try scrolling when selection is not attempted */
    if (ret == MENU_NO_ACTION) ret = menu_scroll();

    if (ret != MENU_NO_ACTION) menu_print();

    return ret;
}