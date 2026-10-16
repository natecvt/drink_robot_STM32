#include "menu_defines.h"

void init_drinks();
void init_ingreds();

MenuItem drinks[ROWS + 2] = {0};
MenuItem ingredients[ROWS + 2] = {0};
Menu calibrate2;

void init_drinks() {
    for (uint8_t i = 0; i < ROWS+2; i++) {
        drinks[i].action = NULL;
        drinks[i].submenu = NULL;
    }
}

void calib2_update_loop()
{
    uint16_t do_update = 1;

    while (!(signal_flags & (SELECT | BACK))) {
        if (do_update) {
            __calibration_type_menu_SH1106_print(&in_ex, cindex);
        }

        /** if inputs are detected and timer has passed */
        do_update = (signal_flags & (RIGHT | LEFT));
        if (signal_flags & RIGHT) stepper_jog(AXIS_X, BUTTON_RIGHT);
        if (signal_flags & LEFT) stepper_jog(AXIS_X, BUTTON_LEFT);
    }

    // if (signal_flags & BACK == BACK);
    // if (signal_flags & SELECT) set_coord(cindex, stepper_report_current_coord(AXIS_X));
}

void init_calib() {
    BIT_SET_MASK(signal_flags, AXES_CANJOG);

    calib2_update_loop();
}

void init_ingreds() {
    /* reset joggability if back is pressed */
    BIT_RESET_MASK(signal_flags, AXES_CANJOG);

    uint16_t cnt = 0;
    get_ingredient_counter(&cnt);
    uint8_t rows = min(ROWS+2, cnt+2);

    for (uint8_t i = 0; i < rows; i++) {
        ingredients[i].action = init_calib;
        ingredients[i].submenu = &calibrate2;
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

Menu calibrate2 = {
    "Calibration",
    NULL, /* different menu style*/
    MENU_ITEMS_PREALLOC,
    &calibrate,
    0, /* TODO: check whether this is a bug*/
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