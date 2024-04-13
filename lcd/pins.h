#ifndef LCD_PINS
#define LCD_PINS

// Reset
#define PIN_ST7565_RST 9

// Data instruction AO
#define PIN_ST7565_A0 7

// Anode of LCD backlight, used with PWM to set the backlight
#define PIN_GPIO_BACKLIGHT 8

// Chip select
#define PIN_ST7565_CS 13

// SPI Transmit / Data out / MOSI
#define PIN_ST7565_TX 11

// SPI Clock
#define PIN_ST7565_SCL 10

// SPI interface
#define SPI_INTERFACE spi1

#endif
