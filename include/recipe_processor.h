#ifndef DRINK_PROCESSOR_H
#define DRINK_PROCESSOR_H

#include "drinks.h"
#include "menu.h"
#include "menu_defines.h"
#include "motion_planner.h"

void rp_find_recipes_list_from_coords();
void calibrate_ingredient(uint8_t ingred_index);

#endif /** DRINK_PROCESSOR_H */