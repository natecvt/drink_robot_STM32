#ifndef MENU_H
#define MENU_H

#ifdef TESTING

#else
    #include "hal_defines.h"
#endif
#include "fonts_local.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define ROWS SH1106_HEIGHT / 10 - 1
#define SUB_INDENT 5
#define NO_ITEMS   UINT8_MAX
#define FONT       &Font_7x10
#define COLOR      SH1106_COLOR_WHITE

typedef struct Menu Menu;

typedef enum {
    MENU_OK = 0,
    MENU_NO_ACTION = 1,
    MENU_LIST_BOUNDS_ERR = 2,
    MENU_NULL_ERR = 3,

} Menu_Err_t;

/** Menus and all related items will be declared in a seperate file */

typedef struct MenuItem {
    char text[18]; /** the name of the item */
    void (*action)(void); /** a handle to the function for actions */
    Menu *submenu; /** the menu resulting from this item's selection */
    
} MenuItem;

/** Menu Items map 
 * {------------displayed items-------------}
 * |   IT0   |   IT1   |   ...   | ITROWS-1 |
 * items is ROWS elements long, referenced by pointer to ensure struct size
*/

struct Menu {
    char title[18]; /** top row title, should be underlined for clarity */
    MenuItem *items; /** an array of current menu items */
    Menu *supmenu; /** the menu above this one */
    uint8_t count; /** count of the number of elements in this menu */
};

extern Menu * cur_menu;

/** not creating the menu, merely priming the relevant objects */
void menu_init(uint8_t going_back);
/** init-ing the top-level menu for startup */
void menu_init_entry(Menu *main);

Menu_Err_t menu_print();
Menu_Err_t menu_update_list(MenuItem *items);

Menu_Err_t menu_scroll();
Menu_Err_t menu_select();
Menu_Err_t menu_signal();

#endif /** MENU_H */
