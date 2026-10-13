#ifndef HAL_DEFINES_H
#define HAL_DEFINES_H

#include <stdint.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "cmsis_gcc.h" /** for __REV and __REV16 if needed */
#include "main.h"

#include "fonts_local.h"

/** --- DEFINES --- */

#define BIT_SET(b, p) (b |= (1 << p))
#define BIT_SET_MASK(b, m) (b |= m)
#define BIT_RESET(b, p) (b &= ~(1 << p))
#define BIT_RESET_MASK(b, m) (b &= ~(m))
/** Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))
/** Minimum */
#define min(a, b) (((a) < (b)) ? (a) : (b))
// idk if this helps, maybe compiler loses optimization ability here
#define S(type) ((uint16_t)sizeof(type))
#define SMALL_NUM 0.01

/** Buttons */
#define DEBOUNCE_MS 50
#define TIMER_ARR_INCR_S 0.0001 // tenth of millisecond
#define TIMER_DIFF (uint16_t)((double)DEBOUNCE_MS / (1000.0 * TIMER_ARR_INCR_S))

extern TIM_HandleTypeDef htim3;
#define BUTTON_TIMER (&htim3)
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

/* AT24CX EEPROM */
#define AT24C512

#define EEPROM_ADDRESS 0x50 << 1
#define EEPROM_WRITE_DELAY 100

#define PAGE_SIZE 128
#define PAGE_EXP 7

#if defined(AT24C1024)
    #define PAGES 1024
#elif defined(AT24C512)
    #define PAGES 512
#elif defined(AT24C256)
    #define PAGES 256
#elif defined(AT24C128)
    #define PAGES 128
#elif defined(AT24C64)
    #define PAGES 64
#elif defined(AT24C32)
    #define PAGES 32
#endif

/** Steppers */
#define STEPPER_NUM 3

#define X_TIMER (&htim4)
#define Y_TIMER (&htim9)
#define Z_TIMER (&htim11)

#define X_RATIO 0.0068 /* mm/step */
#define Y_RATIO -0.0330 /* mm/step */
#define Z_RATIO 0.0050 /* mL/step */

#define X_LIMIT 900.0 /* mm */
#define Y_LIMIT 300.0 /* mm */

/** --- DATA --- */

/** 
 * signals can be set and reset from anywhere
 * for buttons, the bit place is shifted left by the button number below
 */
enum Signals {
    SELECT =     0b00000001,
    RIGHT =      0b00000010,
    DOWN =       0b00000100,
    UP =         0b00001000,
    LEFT =       0b00010000,
    BACK =       0b00001001, /** experimental */
    DATA_FETCH = 0b00100000,
    X_HOME =     0b01000000, /** so far not needed */
    Y_HOME =     0b10000000  /** so far not needed */
};

extern volatile uint8_t signal_flags;

/** Buttons */

/** 
 * when adding more buttons, update __signals_switch with new case
 * 
 */
enum Buttons {
    BUTTON_SEL = 0,
    BUTTON_RIGHT = 1,
    BUTTON_DOWN = 2,
    BUTTON_UP = 3,
    BUTTON_LEFT = 4,
    BUTTON_X_HOME = 5,
    BUTTON_Y_HOME = 6,
    BUTTON_LEN = 7
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

uint8_t SH1106_Init(void);
void SH1106_UpdateScreen(void);
void SH1106_ToggleInvert(void);
void SH1106_Fill(SH1106_COLOR_t Color);
void SH1106_DrawPixel(uint16_t x, uint16_t y, SH1106_COLOR_t color);
void SH1106_GotoXY(uint16_t x, uint16_t y);
char SH1106_Putc(char ch, FontDef_t* Font, SH1106_COLOR_t color);
char SH1106_Puts(char* str, FontDef_t* Font, SH1106_COLOR_t color);
void SH1106_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SH1106_COLOR_t c);
void SH1106_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1106_COLOR_t c);
void SH1106_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1106_COLOR_t c);
void SH1106_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1106_COLOR_t color);
void SH1106_DrawCircle(int16_t x0, int16_t y0, int16_t r, SH1106_COLOR_t c);
void SH1106_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SH1106_COLOR_t c);
void SH1106_I2C_Init();
void SH1106_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);
void SH1106_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);
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

/** AT24CX EEPROM */

typedef enum {
    EEPROM_OK = 0,
    EEPROM_ADDRESS_ERR = 1,
    EEPROM_WRITE_ERR = 2,
    EEPROM_READ_ERR = 3,
    EEPROM_OTHER_ERR = 4
} EEPROM_Status_t;

EEPROM_Status_t eeprom_init();
/** for some reason the EEPROM doesn't expose the address, don't use the following function */
EEPROM_Status_t eeprom_current_address_read(uint8_t *byte_address);
EEPROM_Status_t eeprom_random_read(uint16_t byte_address, uint8_t *data, uint16_t size);
EEPROM_Status_t eeprom_page_write(uint16_t byte_address, uint8_t *data, uint16_t size);

/** Stepper motors */

typedef enum {
    STEPPER_OK = 0,
    STEPPER_INIT_ERR = 1,

} Stepper_Err_t;

enum Axes {
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2,
    AXIS_A = 3,
    AXIS_B = 4,
    AXES_LEN = 5,
};

typedef struct {
    GPIO_TypeDef *en_bus;
    GPIO_TypeDef *dir_bus;
    GPIO_TypeDef *stp_bus;

    uint16_t en_gpio;
    uint16_t dir_gpio;
    uint16_t stp_gpio;

    double ratio;

    uint8_t limit_index;

    TIM_HandleTypeDef *timer;
} Stepper_Init_t;

typedef struct {
    GPIO_TypeDef *en_bus;
    GPIO_TypeDef *dir_bus;
    GPIO_TypeDef *stp_bus;

    uint16_t en_gpio;
    uint16_t dir_gpio;
    uint16_t stp_gpio;

    double ratio;

    uint8_t limit_index;

    TIM_HandleTypeDef *timer;

    uint16_t spr;

    /** EN, DIR */
    GPIO_PinState states[2];
    uint32_t stepcount;

    uint8_t homed;
    double coord;

} Stepper_t;

extern Stepper_t steppers[STEPPER_NUM];

Stepper_Err_t stepper_init(uint8_t axis, Stepper_Init_t *stepper, uint16_t steps_per_rotation);
void stepper_step(uint8_t axis);
void stepper_set_speed(uint8_t axis, uint16_t rpm);
void stepper_set_direction(uint8_t axis, GPIO_PinState dir);
void stepper_set_enable(uint8_t axis, GPIO_PinState en);
void stepper_update_state(uint8_t axis, double dist, uint16_t rpm, GPIO_PinState en);
void stepper_block_until_complete(uint8_t axis);
double stepper_report_current_coord(uint8_t axis);
void stepper_home(uint8_t axis);

#endif /** HAL_DEFINES_H */

