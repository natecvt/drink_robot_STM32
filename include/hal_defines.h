#ifndef HAL_DEFINES_H
#define HAL_DEFINES_H

#include "stm32f4xx_hal.h"
#include "main.h"

#include "fonts_local.h"

#include <stdint.h>
#include <string.h>

/** --- DEFINES --- */

#define BIT_SET(b, p) (b |= (1 << p))
#define BIT_RESET(b, p) (b &= ~(1 << p))
/** Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))
/** Minimum */
#define min(a, b) (((a) < (b)) ? (a) : (b))

/** Buttons */

#define DEBOUNCE_MS 50
#define TIMER_ARR_INCR_S 0.0001 // tenth of millisecond
#define TIMER_DIFF (uint16_t)((double)DEBOUNCE_MS / (1000.0 * TIMER_ARR_INCR_S))
#define BUTTON_TIMER TIM3
#define BUTTON_GPIO_BUS GPIOB

/** how many debounce periods for latch to be reset */
#define SELECT_LATCH_MULT 10
#define SCROLL_LATCH_MULT 2
#define LR_LATCH_MULT 0

/** SH1106 LCD Screen */

/* I2C address */
#define SH1106_I2C_ADDR   0x3C<<1

/* SH1106 settings */
/* SH1106 width in pixels */
#define SH1106_WIDTH          128
/* SH1106 LCD height in pixels */
#define SH1106_HEIGHT          64

#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_INVERTDISPLAY 0xA7

#define SH1106_I2C_TIMEOUT  20000

/* Write command */
#define SH1106_WRITECOMMAND(command)      SH1106_I2C_Write(SH1106_I2C_ADDR, 0x00, (command))
/* Write data */
#define SH1106_WRITEDATA(data)            SH1106_I2C_Write(SH1106_I2C_ADDR, 0x40, (data))

/** --- DATA --- */
/** Buttons */

enum Buttons {
    BUTTON_SEL = 0,
    BUTTON_RIGHT = 1,
    BUTTON_DOWN = 2,
    BUTTON_UP = 3,
    BUTTON_LEFT = 4,
    BUTTON_LEN = 5
};

enum Signals {
    SELECT = 0b00000001,
    RIGHT =  0b00000010,
    DOWN =   0b00000100,
    UP =     0b00001000,
    LEFT =   0b00010000,
    BACK =   0b00001001, /** experimental */
};

typedef enum {
    BUTTON_LOW = GPIO_PIN_RESET,
    BUTTON_HIGH = GPIO_PIN_SET,
} Button_State_t;

typedef enum {
    BUTTON_HAL_OK = 0,
    BUTTON_HAL_PROTECTED = 1,
    BUTTON_HAL_UNKNOWN = 2,
} Button_HAL_Err_t;

/** #TODO: resolve HAL/native button interdependency */
typedef struct {
    /** gpio pins used by the button */
    GPIO_TypeDef* gpio_bus;
    uint16_t gpio;

    uint8_t protected;
    uint8_t signal_latched;
    Button_State_t state;
} Button_HAL_t;

extern Button_HAL_t buttons_HAL[];
extern volatile uint8_t signal_flags;

void button_HAL_timer_init();
void button_HAL_init(Button_HAL_t *button, uint16_t gpio);
uint8_t button_HAL_index_from_gpio(uint16_t gpio);
Button_HAL_Err_t buttons_HAL_isr(uint8_t button_index);
void buttons_HAL_reset();

/** SH1106 LCD Screen */

typedef enum {
	SH1106_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SH1106_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SH1106_COLOR_t;

/**
 * @brief  Initializes SH1106 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
uint8_t SH1106_Init(void);

/** 
 * @brief  Updates buffer from internal RAM to LCD
 * @note   This function must be called each time you do some changes to LCD, to update buffer from RAM to LCD
 * @param  None
 * @retval None
 */
void SH1106_UpdateScreen(void);

/**
 * @brief  Toggles pixels invertion inside internal RAM
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  None
 * @retval None
 */
void SH1106_ToggleInvert(void);

/** 
 * @brief  Fills entire LCD with desired color
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval None
 */
void SH1106_Fill(SH1106_COLOR_t Color);

/**
 * @brief  Draws pixel at desired location
 * @note   @ref SH1106_UpdateScreen() must called after that in order to see updated LCD screen
 * @param  x: X location. This parameter can be a value between 0 and SH1106_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SH1106_HEIGHT - 1
 * @param  color: Color to be used for screen fill. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval None
 */
void SH1106_DrawPixel(uint16_t x, uint16_t y, SH1106_COLOR_t color);

/**
 * @brief  Sets cursor pointer to desired location for strings
 * @param  x: X location. This parameter can be a value between 0 and SH1106_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SH1106_HEIGHT - 1
 * @retval None
 */
void SH1106_GotoXY(uint16_t x, uint16_t y);

/**
 * @brief  Puts character to internal RAM
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval Character written
 */
char SH1106_Putc(char ch, FontDef_t* Font, SH1106_COLOR_t color);

/**
 * @brief  Puts string to internal RAM
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char SH1106_Puts(char* str, FontDef_t* Font, SH1106_COLOR_t color);

/**
 * @brief  Draws line on LCD
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x0: Line X start point. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y0: Line Y start point. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  x1: Line X end point. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y1: Line Y end point. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval None
 */
void SH1106_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SH1106_COLOR_t c);

/**
 * @brief  Draws rectangle on LCD
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval None
 */
void SH1106_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1106_COLOR_t c);

/**
 * @brief  Draws filled rectangle on LCD
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval None
 */
void SH1106_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1106_COLOR_t c);

/**
 * @brief  Draws triangle on LCD
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x1: First coordinate X location. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y1: First coordinate Y location. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  x2: Second coordinate X location. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y2: Second coordinate Y location. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  x3: Third coordinate X location. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y3: Third coordinate Y location. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval None
 */
void SH1106_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1106_COLOR_t color);

/**
 * @brief  Draws circle to STM buffer
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval None
 */
void SH1106_DrawCircle(int16_t x0, int16_t y0, int16_t r, SH1106_COLOR_t c);

/**
 * @brief  Draws filled circle to STM buffer
 * @note   @ref SH1106_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to SH1106_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to SH1106_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SH1106_COLOR_t enumeration
 * @retval None
 */
void SH1106_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SH1106_COLOR_t c);

/**
 * @brief  Initializes SH1106 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
void SH1106_I2C_Init();

/**
 * @brief  Writes single byte to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
void SH1106_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);

/**
 * @brief  Writes multi bytes to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
void SH1106_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/**
 * @brief  Draws the Bitmap
 * @param  X:  X location to start the Drawing
 * @param  Y:  Y location to start the Drawing
 * @param  *bitmap : Pointer to the bitmap
 * @param  W : width of the image
 * @param  H : Height of the image
 * @param  color : 1-> white/blue, 0-> black
 */
void SH1106_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);

// scroll the screen for fixed rows
void SH1106_ScrollRight(uint8_t start_row, uint8_t end_row);
void SH1106_ScrollLeft(uint8_t start_row, uint8_t end_row);
void SH1106_Scrolldiagright(uint8_t start_row, uint8_t end_row);
void SH1106_Scrolldiagleft(uint8_t start_row, uint8_t end_row);
void SH1106_Stopscroll(void);
void SH1106_ScrollDown(uint8_t start_row, uint8_t end_row);

// inverts the display i = 1->inverted, i = 0->normal
void SH1106_InvertDisplay (int i);

// clear the display
void SH1106_Clear (void);

#endif /** HAL_DEFINES_H */
