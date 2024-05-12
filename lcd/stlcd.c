/*
$Id:$

ST7565 LCD library!

Copyright (C) 2010 Limor Fried, Adafruit Industries

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 // some of this code was written by <cstone@pobox.com> originally; it is in the
public domain.
*/

/* 
Converted by: Bruce Botes - 18 Dec 2023

My very first attempt using the Pi Pico Toolchain with a LCD (128x64) and spi.
Code adapted for use in the Pi Pico C/C++ development environment.
The original code is 13 years old - tried and tested. 
Very basic C and easy to convert to the Pi Pico C/C++toolchain.

NOTE the GMG12864 and PiPico are using 3.3v TTL
Pin mapping
---------------------------------------------------------------------------------------
GMG12864                                                <-> PiPico
---------------------------------------------------------------------------------------
1 - CS(chip select)                                     <-> GPIO 17 / SPI0 - CSN (pin 22)
2 - Res(Reset LCD)                                      <-> GPIO 20 (pin 26)
3 - RS(also AO - comand data instruction)               <-> GPIO 21 (pin 27) - not used (future )
4 - SCL(also SCK/SCLK - spi clock)                      <-> GPIO 18 / SPI0 - SCK (pin 24)
5 - SI(also SDA/MOSI                                    <-> GPIO 19 / SPI0 - MOSI/TX (pin 25)
6 - VDD(3.3V - I guess for the ST7565 onboard chip)     <-> 3.3v (pin 36)
7 - VSS(GND)                                            <-> GND (pin 38)
8 - A (Backlight Anode - 3.3V)                          <-> 3.3v (pin 36) 
9 - K (Backlight Cathode - GND)                         <-> GND (pin 38)
10 to 13 ( SPI for Chinese characterset)                <-> not used
*/
#include "stlcd.h"
#include "glcd.h"
#include "pins.h"
#include <hardware/spi.h>
#include <hardware/gpio.h>

// JP: unused:?
//uint8_t is_reversed = 0;

// int pagemap[] = {3, 2, 1, 0, 7, 6, 5, 4};
int pagemap[] = {7, 6, 5, 4, 3, 2, 1, 0};

const static unsigned char logo16_glcd_bmp[]={
0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0x30, 0xf8, 0xbe, 0x9f, 0xff, 0xf8, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 
0x20, 0x3c, 0x3f, 0x3f, 0x1f, 0x19, 0x1f, 0x7b, 0xfb, 0xfe, 0xfe, 0x07, 0x07, 0x07, 0x03, 0x00, };

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_ST7565_CS, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_ST7565_CS, 1);
    asm volatile("nop \n nop \n nop");
}

void enable_Pico_pins(void) {
  // JP: Merge backlight pwm code here
  
  // Hard reset pin on 12864 LCD
  gpio_init(PIN_ST7565_RST);
  gpio_set_dir(PIN_ST7565_RST, GPIO_OUT);
  
  // Data instruction on 12864 (switch between command or data mode)
  gpio_init(PIN_ST7565_A0);
  gpio_set_dir(PIN_ST7565_A0, GPIO_OUT);

  // This example will use SPI_INTERFACE at 0.5MHz.
  spi_init(SPI_INTERFACE, 500 * 1000);
  // JP: ST7565 isn't full duplex 
  // gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PIN_ST7565_SCL, GPIO_FUNC_SPI);
  gpio_set_function(PIN_ST7565_TX, GPIO_FUNC_SPI);

  // Chip select is active-low, so we'll initialise it to a driven-high state
  gpio_init(PIN_ST7565_CS);
  gpio_set_dir(PIN_ST7565_CS, GPIO_OUT);
  gpio_put(PIN_ST7565_CS, 1);
}

void clear_screen(void) {
  uint8_t p, c;

  for (p = 0; p < 8; p++) {
    st7565_command(CMD_SET_PAGE | p);
    for (c = 0; c < 128; c++) {
      st7565_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      st7565_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      st7565_data(0x0);
    }
  }
}

void st7565_init(void) {
  // JP: integrate pwm backlight
  // enable Black light
  // gpio_put(BLA_PIN, 1); future

  // toggle RST low to reset; CS low so it'll listen to us
  cs_select();
  gpio_put(PIN_ST7565_RST, 0); // set low
  sleep_ms(500);
  gpio_put(PIN_ST7565_RST, 1); // set high

  // LCD bias select
  st7565_command(CMD_SET_BIAS_7);
  // ADC select
  st7565_command(CMD_SET_ADC_NORMAL);
  // SHL select
  st7565_command(CMD_SET_COM_NORMAL);
  // Initial display line
  st7565_command(CMD_SET_DISP_START_LINE);
  // Set reverse display
  //st7565_command(CMD_SET_DISP_REVERSE);

  // turn on voltage converter (VC=1, VR=0, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x4);
  // wait for 50% rising
  sleep_ms(50);

  // turn on voltage regulator (VC=1, VR=1, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x6);
  // wait >=50ms
  sleep_ms(50);

  // turn on voltage follower (VC=1, VR=1, VF=1)
  st7565_command(CMD_SET_POWER_CONTROL | 0x7);
  // wait
  sleep_ms(50);

  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  st7565_command(CMD_SET_RESISTOR_RATIO | 0x6);
}


inline void spiwrite(uint8_t c) {
  cs_select();
  spi_write_blocking(SPI_INTERFACE, &c, 1);
  cs_deselect();
}

// write a command to the ST7565 SPI controller
void st7565_command(uint8_t c) {
  gpio_put(PIN_ST7565_A0, 0); //set low for command

  spiwrite(c);
}

// write display data to the ST7565 SPI controller
void st7565_data(uint8_t c) {
  gpio_put(PIN_ST7565_A0, 1); // set high for data

  spiwrite(c);
}
void st7565_set_brightness(uint8_t val) {
  st7565_command(CMD_SET_VOLUME_FIRST);
  st7565_command(CMD_SET_VOLUME_SECOND | (val & 0x3f));
}

void write_buffer(uint8_t *buffer) {
  uint8_t c, p;

  for (p = 0; p < 8; p++) {
    st7565_command(CMD_SET_PAGE | pagemap[p]);
    st7565_command(CMD_SET_COLUMN_LOWER | (0x0 & 0xf));
    st7565_command(CMD_SET_COLUMN_UPPER | ((0x0 >> 4) & 0xf));
    st7565_command(CMD_RMW); // not sure if this is really necessary??

    for (c = 0; c < 128; c++) {
      st7565_data(buffer[(128 * p) + c]);
    }
  }
}
