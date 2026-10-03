#include "eeprom_programmer.h"

/** max size expected from transmission */
uint8_t rx_buf[BUF_SIZE];
uint8_t rx_ele = 0, cnt = 0, err = 0;

Drink_Err_t set_ingredient_counter(const uint16_t counter)
{
    uint16_t add = IA_REGION << PAGE_EXP;

    return eeprom_page_write(add, (uint8_t *)&counter, S(IAccount));
}

Drink_Err_t add_ingredient(char* name)
{
    uint16_t index, address;
    uint8_t size = (uint8_t)strlen(name) + 1; // add 1 to keep the null terminator
    /** force null termination within bounds */
    if (size > MAX_NAMEC) name[MAX_NAMEC] = '\0';

    /** get the new index and address */
    if (get_ingredient_counter(&index)) return DRINK_OTHER_ERROR;
    if (get_ingredient_address(&address, index)) return DRINK_ADDRESS_ERROR;

    /** write I data to first open I spot */
    EEPROM_Status_t erc = eeprom_page_write(address, (uint8_t *)&in_ex, size);
    if (erc == EEPROM_ADDRESS_ERR) {
        /** address is set to the start of the next page */
        address = ((address >> PAGE_EXP) + 1) << PAGE_EXP;
        /** try again */
        erc = eeprom_page_write(address, (uint8_t *)&in_ex, size);
        if (erc) return DRINK_WRITE_ERROR;
    }
    else if (erc) return DRINK_WRITE_ERROR;

    address += size;
    /** setting the new counter = index + 1 */
    index++;
    if (set_ingredient_counter(index)) return DRINK_OTHER_ERROR;

    /** write address to first open IA spot */
    index = index * S(IAccount) + (IA_REGION << PAGE_EXP);
    if (eeprom_page_write(index, (uint8_t *)&address, S(IAccount))) return DRINK_ADDRESS_ERROR;

    return DRINK_OK;
}

Drink_Err_t add_ingredient_pre() 
{
    return add_ingredient(in_ex.name);
}

Drink_Err_t set_recipe_counter(const uint16_t counter)
{
    uint16_t add = RA_REGION << PAGE_EXP;

    return eeprom_page_write(add, (uint8_t *)&counter, S(IAccount));
}

Drink_Err_t add_recipe(char *name, const uint8_t ningred, uint8_t* const ingred_amounts, uint8_t* const ingred_indexes)
{
    uint16_t index, address;
    uint8_t size = (uint8_t)strlen(name) + 1; // add 1 to keep the null terminator
    /** force null termination within bounds */
    if (size > MAX_NAMEC) name[MAX_NAMEC] = '\0';
    size += 1 + 2 * ningred;

    if (name != rp_ex.name) {
        strcpy(rp_ex.name, name);
        rp_ex.ningred = ningred;
        memcpy(rp_ex.ingred_amounts, ingred_amounts, ningred);
        memcpy(rp_ex.ingred_indexes, ingred_indexes, ningred);
    }

    /** get the new index and address */
    if (get_recipe_counter(&index)) return DRINK_OTHER_ERROR;
    if (get_recipe_address(&address, index)) return DRINK_ADDRESS_ERROR;

    /** if page overflow would happen, go to next */
    if (address >> PAGE_EXP != (address + size) >> PAGE_EXP) {
        address = ((address + size) >> PAGE_EXP) << PAGE_EXP;
    }
    /** write R data to first open R spot, sheesh */
    EEPROM_Status_t erc = eeprom_page_write(address, (uint8_t *)&rp_ex.ningred, 1);
    if (erc) return DRINK_WRITE_ERROR;
    erc = eeprom_page_write(address+1, (uint8_t *)&rp_ex.ingred_amounts, rp_ex.ningred);
    if (erc) return DRINK_WRITE_ERROR;
    erc = eeprom_page_write(address+1+rp_ex.ningred, (uint8_t *)&rp_ex.ingred_indexes, rp_ex.ningred);
    if (erc) return DRINK_WRITE_ERROR;
    erc = eeprom_page_write(address+1+2*rp_ex.ningred, (uint8_t *)&rp_ex.name, strlen(rp_ex.name) + 1);
    if (erc) return DRINK_WRITE_ERROR;

    address += size;
    /** setting the new counter = index + 1 */
    index++;
    if (set_recipe_counter(index)) return DRINK_OTHER_ERROR;

    /** write address to first open IA spot */
    index = index * S(RAccount) + (RA_REGION << PAGE_EXP);
    if (eeprom_page_write(index, (uint8_t *)&address, S(RAccount))) return DRINK_ADDRESS_ERROR;

    return DRINK_OK;
}

Drink_Err_t add_recipe_pre()
{
    return add_recipe(rp_ex.name, rp_ex.ningred, rp_ex.ingred_amounts, rp_ex.ingred_indexes);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart != PROG_UART) return;

    char c = rx_ele;
    HAL_UART_Receive_IT(PROG_UART, &rx_ele, 1);

    if (c == '\n' || c == '\r') {
        cnt = 0;
        eeprom_programmer_incoming_data_interpret();
    }
    else {
        rx_buf[cnt] = c;
        if (cnt++ == BUF_SIZE) return;
    }
}

uint8_t eeprom_programmer_init()
{
    /** this can be called multiple times without problem */
    if (eeprom_init()) return 1;

    if (HAL_UART_GetState(PROG_UART) != HAL_UART_STATE_READY) return 1;

    if (set_ingredient_counter(0)) return 1;
    if (set_recipe_counter(0)) return 1;
    memset(rx_buf, 0, BUF_SIZE);

    return 0;
}

uint8_t eeprom_programmer_incoming_data_interpret()
{
    
    /** if first character is a number, then data is for a recipe */
    uint8_t is_recipe = rx_buf[0] < 10;

    if (is_recipe) {
        memset(&rp_ex, 0, S(Recipe));
        rp_ex.ningred = rx_buf[0];

        memcpy(rp_ex.ingred_amounts, &rx_buf[1], rp_ex.ningred);
        memcpy(rp_ex.ingred_indexes, &rx_buf[1+rp_ex.ningred], rp_ex.ningred);
        strcpy(rp_ex.name, (char*)&rx_buf[1+2*rp_ex.ningred]);

        eeprom_programmer_add_item(1);
    }
    else {
        memset(&in_ex, 0, S(Ingredient));
        strcpy(in_ex.name, (char*)rx_buf);

        eeprom_programmer_add_item(0);
    }
    
    memset(rx_buf, 0, BUF_SIZE);
    return 0;
}

uint8_t eeprom_programmer_add_item(uint8_t is_recipe)
{
    static uint8_t erc = 0;

    if (is_recipe) {
        erc = add_recipe_pre();
    }
    else {
        erc = add_ingredient_pre();
    }

    if (!erc) erc = HAL_UART_Transmit(PROG_UART, (uint8_t*)RCPT_MSG, RCPT_SIZ, 20000);

    return erc;
}