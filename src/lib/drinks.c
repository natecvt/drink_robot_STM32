#include "drinks.h"

Ingredient in_ex = {0};
Recipe rp_ex = {0};
ICoord cd_ex = {0};
IAccount ia_ex = {0};
RAccount ra_ex = {0};
uint8_t cindex = 0;

Drink_Err_t get_ingredient(uint16_t index)
{
    uint16_t counter, address;

    uint8_t erc = get_ingredient_counter(&counter);
    if (erc) return erc;
    if (index >= counter) return DRINK_ADDRESS_ERROR;

    erc = get_ingredient_address(&address, index);
    if (erc) return erc;

    /**
     * The following will read over to the next ingredient, but should keep the null terminator,
     * string methods will still work on the name if this occurs
     */
    erc = eeprom_random_read(address, (uint8_t*)&in_ex, S(Ingredient));
    if (erc) return DRINK_GET_ERROR;

    return DRINK_OK;
}

Drink_Err_t get_ingredient_address(uint16_t* const address, const uint16_t index)
{
    uint16_t counter, byte;

    if (index == 0) {
        *address = IN_REGION << PAGE_EXP;
        return DRINK_OK;
    }

    if (get_ingredient_counter(&counter)) return DRINK_GET_ERROR;

    /** counter is the exclusive limit on index */
    if (index > counter) return DRINK_OTHER_ERROR;

    /** first spot starts at the page defined by IN_REGION */
    byte = index * S(IAccount);

    if (eeprom_random_read(byte, 
        (uint8_t *)address, 
        S(IAccount))) return DRINK_ADDRESS_ERROR;

    return DRINK_OK;
}

Drink_Err_t get_ingredient_counter(uint16_t* const counter)
{
    uint16_t add = IA_REGION << PAGE_EXP;

    return eeprom_random_read(add, (uint8_t *)counter, S(IAccount));
}

Drink_Err_t get_recipe(const uint16_t index)
{
    uint16_t counter, address;

    uint8_t erc = get_recipe_counter(&counter);
    if (erc) return erc;
    if (index > counter) return DRINK_ADDRESS_ERROR;

    erc = get_recipe_address(&address, index);
    if (erc) return erc;

    /**
     * The following will read over to the next ingredient, but should keep the null terminator,
     * string methods will still work on the name if this occurs
     */
    erc = eeprom_random_read(address, (uint8_t*)&rp_ex, S(Ingredient));
    if (erc) return DRINK_GET_ERROR;

    return DRINK_OK;
}

Drink_Err_t get_recipe_address(uint16_t* const address, const uint16_t index)
{
    uint16_t counter, byte;

    if (index == 0) {
        *address = RP_REGION << PAGE_EXP;
        return DRINK_OK;
    }

    if (get_recipe_counter(&counter)) return DRINK_GET_ERROR;

    /** counter is the exclusive limit on index */
    if (index >= counter) return DRINK_OTHER_ERROR;

    /** first spot starts at the page defined by IN_REGION */
    byte = index * S(IAccount);

    if (eeprom_random_read(byte, 
        (uint8_t *)address, 
        S(IAccount))) return DRINK_ADDRESS_ERROR;

    return DRINK_OK;
}

Drink_Err_t get_recipe_counter(uint16_t* const counter)
{
    uint16_t add = RA_REGION << PAGE_EXP;

    return eeprom_random_read(add, (uint8_t *)counter, S(IAccount));
}

Drink_Err_t set_coord(const uint16_t ingred_index, const double coord)
{
    uint16_t add = (CD_REGION << PAGE_EXP) + ingred_index * S(ICoord);

    return eeprom_page_write(add, (uint8_t*)&coord, S(ICoord));
}

Drink_Err_t get_coord(const uint16_t ingred_index)
{
    uint16_t add = (CD_REGION << PAGE_EXP) + ingred_index * S(ICoord);

    return eeprom_random_read(add, (uint8_t*)&cd_ex, S(ICoord));
}

void calc_proportions(double *proportions)
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < rp_ex.ningred; i++) {
        sum += rp_ex.ingred_amounts[i];
    }

    for (uint8_t i = 0; i < rp_ex.ningred; i++) {
        proportions[i] = (double)rp_ex.ingred_amounts[i] / (double)sum;
    }
}

void calc_amounts(double *amounts, double *proportions, double amount)
{
    for (uint8_t i = 0; i < rp_ex.ningred; i++) {
        amounts[i] = proportions[i] * amount;
    }
}