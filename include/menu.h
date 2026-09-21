#ifndef MENU_H
#define MENU_H

#ifdef TESTING

#else
    #include "hal_defines.h"
#endif
#include "fonts_local.h"

#include <memory.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define ROWS SH1106_HEIGHT / 10 - 1
#define SUB_INDENT 5
#define NO_ITEMS   UINT8_MAX
#define FONT       &Font_7x10
#define COLOR      SH1106_COLOR_WHITE

typedef enum {
    MENU_OK = 0,
    MENU_NO_ACTION = 1,
    MENU_LIST_BOUNDS_ERR = 2,
    MENU_NULL_ERR = 3,

} Menu_Err_t;

enum Menu_Items_Alloc_Method {
    MENU_ITEMS_PREALLOC = 0,
    MENU_ITEMS_PROCEDUR = 1
};

/** Menus and all related items will be declared in a seperate file */
typedef struct Menu Menu;

typedef struct MenuItem {
    char text[18]; /** the name of the item */
    void (*action)(void); /** a handle to the function for actions */
    Menu *submenu; /** the menu resulting from this item's selection */
    
} MenuItem;

/** Menu Items map for longer list than can be displayed
 * {----------------------underlying data----------------------}
 * |  data0  |   ...   |   ...   |   ...   |   ...   |   ...   |
 * |----menu_index---->{------------displayed items------------}
 *                     |   IT0   |   IT1   |   ...   | ITROWS-1|
 * items is ROWS elements long, referenced by pointer
*/

struct Menu {
    char title[18]; /** top row title, should be underlined for clarity */
    MenuItem *items; /** an array of current menu items */
    uint8_t item_regime; /** item alloc regime, see Menu_Items_Alloc_Method enum */
    Menu *supmenu; /** the menu above this one */
    uint8_t count; /** count of the number of elements in this menu */
};

extern Menu * cur_menu;
extern MenuItem menu_manip[ROWS]; /** external array used to init procedural menus */

/** not creating the menu, merely priming the relevant objects */
void menu_init(uint8_t going_back);
/** init-ing the top-level menu for startup */
void menu_init_entry(Menu *main);

Menu_Err_t menu_print();

/**
 * @brief updates current list contents
 */
Menu_Err_t menu_update_list(uint8_t up);

Menu_Err_t menu_scroll();
Menu_Err_t menu_select();
Menu_Err_t menu_signal();

#endif /** MENU_H */
