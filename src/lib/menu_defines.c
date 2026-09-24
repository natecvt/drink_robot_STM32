#include "menu_defines.h"

MenuItem main_items[MAIN_N] = {
    {"Select Drink", init_drinks, &select_drink},
    {"Add Recipe", NULL, NULL},
    {"Add Ingredient", NULL, NULL},
    {"Calibrate", NULL, NULL},
    {"Settings", NULL, NULL},
};

MenuItem drinks[ROWS + 2] = {0};

void init_drinks() {
    for (uint8_t i = 0; i < ROWS+2; i++) {
        drinks[i].action = NULL;
        drinks[i].submenu = NULL;
        strcpy(drinks[i].text, "Hi");
    }
}

Menu main_menu = {
    "Main Menu",
    main_items,
    MENU_ITEMS_PREALLOC,
    NULL,
    MAIN_N
};

Menu select_drink = {
    "Select Drink",
    drinks,
    MENU_ITEMS_PREALLOC,
    &main_menu,
    ROWS+2 /** add 2 buffer items for front and back */
};

