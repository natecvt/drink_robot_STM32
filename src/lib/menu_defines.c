#include "menu_defines.h"

void init_drinks();
void init_ingreds();

MenuItem drinks[ROWS + 2] = {0};
MenuItem ingredients[ROWS + 2] = {0};

void init_drinks() {
    for (uint8_t i = 0; i < ROWS+2; i++) {
        drinks[i].action = NULL;
        drinks[i].submenu = NULL;
    }
}

void init_ingreds() {
    for (uint8_t i = 0; i < ROWS+2; i++) {
        ingredients[i].action = NULL;
        ingredients[i].submenu = NULL;
    }
}

Menu select_drink = {
    "Select Drink",
    drinks,
    MENU_ITEMS_PROCEDUR,
    &main_menu,
    ROWS+2 /** add 2 buffer items for front and back */
};

Menu calibrate = {
    "Select Ingredient",
    ingredients,
    MENU_ITEMS_PROCEDUR,
    &main_menu,
    ROWS+2
};

MenuItem main_items[MAIN_N] = {
    {"Select Drink", init_drinks, &select_drink},
    {"Add Recipe", NULL, NULL},
    {"Calibrate", init_ingreds, &calibrate},
    {"Settings", NULL, NULL},
};

Menu main_menu = {
    "Main Menu",
    main_items,
    MENU_ITEMS_PREALLOC,
    NULL,
    MAIN_N
};