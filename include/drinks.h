#ifndef DRINKS_H
#define DRINKS_H

#include "hal_defines.h"

/** sizes of specific items within structures */
#define MAX_NINGS 8
#define MAX_NAMEC 18 /** max number of characters displayable on the screen */
#define MAX_RECIB MAX_NAMEC + 2 * MAX_NINGS + 1

/** start of memory regions, in 128B pages */
    #define IA_REGION 0
    #define RA_REGION 4
    #define IN_REGION 8
    #define RP_REGION 136
    #define CD_REGION 392

/** EEPROM Mapping for Objects
 * IA_REGION -- all blocks shown are 2 bytes | *a block* |
 * {| icnt | add1 | add2 | .... | addN | XXXX | ...
 *     ^ icnt = N, IA_REGION byte 2N is the last account location
 * 
 * IN_REGION -- blocks are indeterminant size, but still contiguous
 * {| ing0 | ing1 | .... | XXXX }{| ingM | ... | ingN | XXXX | ...
 * ^              ^       ^ ingM didn't fit within the page, onto next
 * |              | add1 stores the location of the first byte in ing2
 * | a page boundary marker, pages are PAGE_SIZE-bytes long, if an
 *   Object overlaps with a boundary, it is placed on the next page,
 *   addM-1 still points to ingM even though it overlaps
 * 
 * similar constructs apply to Recipes
 */

typedef enum {
    DRINK_OK = 0,
    DRINK_ADDRESS_ERROR = 1,
    DRINK_GET_ERROR = 2,
    DRINK_WRITE_ERROR = 3,
    DRINK_OTHER_ERROR = 4,
    DRINK_CALIB_ERROR = 5,
    DRINK_PROCESS_ERROR = 6
} Drink_Err_t;

typedef struct {
    char name[MAX_NAMEC];
} Ingredient;

typedef struct {
    uint8_t ningred;
    uint8_t ingred_amounts[MAX_NINGS];
    uint8_t ingred_indexes[MAX_NINGS];
    char name[MAX_NAMEC];
} Recipe;

typedef struct {
    double coord;
} ICoord;

/** counter is one of these, but it is the current max index */
typedef struct {
    uint8_t address[2];
} IAccount;

/** counter is one of these, but it is the current max index */
typedef struct {
    uint8_t address[2];
} RAccount;

/** -- Details on the various names used below: --
 *  index: 2 * (index + 1) is the offset where one could find the Account for an Object,
 *      the account gives the address where the Object is located
 *  address: the 2-byte location of the next Object's first byte stored on the EEPROM,
 *      for index i, the address at byte 2*(i+1) points to Object i+1's first byte
 *      if i=0, the address for the Object is the start of the Object region,
 *      freeing the first slot in the Account region for the counter
 *  counter: the upper limit on the index, the current number of Objects stored,
 *      a value located at the start of the Account regions
 *  these definitions apply to both Recipes and Ingredients and pertain to the EEPROM mapping
 */

/** #TODO: maybe relocate adding/setting functions to the eeprom programmer script */
Drink_Err_t add_ingredient(char* name);
Drink_Err_t add_ingredient_pre();
Drink_Err_t get_ingredient(uint16_t index);
Drink_Err_t get_ingredient_address(uint16_t* const address, const uint16_t index);
Drink_Err_t set_ingredient_counter(const uint16_t counter);
Drink_Err_t get_ingredient_counter(uint16_t* const counter);

Drink_Err_t add_recipe(char *name, const uint8_t ningred, uint8_t* const ingred_amounts, uint8_t* const ingred_indexes);
Drink_Err_t add_recipe_pre();
Drink_Err_t get_recipe(const uint16_t index);
Drink_Err_t get_recipe_address(uint16_t* const address, const uint16_t index);
Drink_Err_t set_recipe_counter(const uint16_t counter);
Drink_Err_t get_recipe_counter(uint16_t* const counter);

Drink_Err_t set_coord(const uint16_t ingred_index, const double coord);
Drink_Err_t get_coord(const uint16_t ingred_index);

/**
 * Equations for these functions:
 * - %ing(x) = 100 * ing_amounts(x) / sum(ing_amounts) [for display]
 * - amount_ing(x) = %ing(x) * amount_queued / 100     [for processing]
 */
void calc_proportions(double *proportions);
void calc_amounts(double *amounts, double *proportions, double amount);

/** globally accessible (working) variables for manipulation */
extern Ingredient in_ex;
extern Recipe rp_ex;
extern ICoord cd_ex;
extern IAccount ia_ex;
extern RAccount ra_ex;

#endif /** DRINKS_H */