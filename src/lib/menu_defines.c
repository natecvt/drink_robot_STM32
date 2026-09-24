#include "menu_defines.h"

MenuItem main_items[MAIN_N] = {
    {"Select Drink", NULL, &select_drink},
    {"Add Recipe", NULL, NULL},
    {"Add Ingredient", NULL, NULL},
    {"Calibrate", NULL, NULL},
    {"Settings", NULL, NULL},
};

MenuItem drinks[ROWS + 2] = {
    {"Hi2", NULL, NULL},
    {"Hi", NULL, NULL},
    {"Hi", NULL, NULL},
    {"Hi", NULL, NULL},
    {"Hi", NULL, NULL},
    {"Hi", NULL, NULL},
    {"Hi1", NULL, NULL}
};

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
    ROWS+2
};

