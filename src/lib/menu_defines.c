#include "menu_defines.h"

MenuItem main_items[MAIN_N] = {
    {"Select Drink", NULL, NULL},
    {"Add Recipe", NULL, NULL},
    {"Add Ingredient", NULL, NULL},
    {"Calibrate", NULL, NULL},
    {"Settings", NULL, NULL},
};

Menu main_menu = {
    "Main Menu",
    main_items,
    NULL,
    MAIN_N
};
