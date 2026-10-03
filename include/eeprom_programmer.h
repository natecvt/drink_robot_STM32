#ifndef EEPROM_PROGRAMMER_H
#define EEPROM_PROGRAMMER_H

#include "hal_defines.h"
#include "drinks.h"

extern UART_HandleTypeDef huart2;
#define PROG_UART &huart2

#define BUF_SIZE S(Recipe)+1

#define RCPT_SIZ 10
#define RCPT_MSG "Received\n"

extern uint8_t rx_ele;

uint8_t eeprom_programmer_init();
uint8_t eeprom_programmer_incoming_data_interpret();
uint8_t eeprom_programmer_add_item(uint8_t is_recipe);

#endif /** EEPROM_PROGRAMMER_H */