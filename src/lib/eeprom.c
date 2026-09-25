#include "hal_defines.h"

extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C &hi2c1

// #TODO: handle endianness difference from incoming bytes
// #TODO: modify write to copy the incoming data to avoid unwanted modifications (also make stuff const)

EEPROM_Status_t eeprom_init()
{
    while (HAL_I2C_IsDeviceReady(EEPROM_I2C, (EEPROM_ADDRESS ), 1, HAL_MAX_DELAY)) {
        HAL_Delay(1);
    }

    return EEPROM_OK;
}

EEPROM_Status_t eeprom_current_address_read(uint8_t *byte_address) 
{
    if (HAL_I2C_Master_Receive(EEPROM_I2C, 
        (EEPROM_ADDRESS ), 
        byte_address, 
        1, 
        HAL_MAX_DELAY) != HAL_OK) {
        return EEPROM_ADDRESS_ERR;
    }
    return EEPROM_OK;
}

EEPROM_Status_t eeprom_random_read(uint16_t byte_address, uint8_t *data, uint16_t size)
{
    uint32_t end = byte_address + size - 1;
    /** address is allowed to go up to page 511 */
    if ((end >> PAGE_EXP) >= PAGES) return EEPROM_OTHER_ERR;

    if (HAL_I2C_Mem_Read(EEPROM_I2C, 
        EEPROM_ADDRESS, 
        byte_address, 
        I2C_MEMADD_SIZE_16BIT, 
        data, 
        size, 
        HAL_MAX_DELAY) != HAL_OK) return EEPROM_READ_ERR;

    return EEPROM_OK;
}

EEPROM_Status_t eeprom_page_write(uint16_t byte_address, uint8_t *data, uint16_t size)
{

    uint32_t end = byte_address + size - 1;
    /** detects overflow */
    if ((end >> PAGE_EXP) >= PAGES) return EEPROM_OTHER_ERR;

    /** detects intra-page overflow */
    if ((byte_address >> PAGE_EXP) - (end >> PAGE_EXP)) return EEPROM_ADDRESS_ERR;

    if (HAL_I2C_Mem_Write(EEPROM_I2C, 
        EEPROM_ADDRESS, 
        byte_address, 
        I2C_MEMADD_SIZE_16BIT, 
        data, 
        size, 
        HAL_MAX_DELAY) != HAL_OK) return EEPROM_WRITE_ERR;

    return EEPROM_OK;
}