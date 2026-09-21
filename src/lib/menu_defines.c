#include "menu_defines.h"

MenuItem main_items[MAIN_N] = {
    {"Select Drink", NULL, NULL},
    {"Add Recipe", NULL, NULL},
    {"Add Ingredient", NULL, NULL},
    {"Calibrate", NULL, NULL},
    {"Settings", NULL, NULL},
    {"Settings2", NULL, NULL},
};

Menu main_menu = {
    "Main Menu",
    main_items,
    MENU_ITEMS_PREALLOC,
    NULL,
    MAIN_N
};
