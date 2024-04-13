#include <display.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <stdio.h>
#include <stlcd.h>
#include <glcd.h>
#include <pins.h>

uint8_t buffer[128 * 64 / 8] = {0};

void init_display(void) {
  printf("Initializing display\n");

  // Tell GPIO 0 and 1 they are allocated to the PWM
  gpio_set_function(PIN_GPIO_BACKLIGHT, GPIO_FUNC_PWM);
  const int slice_num = pwm_gpio_to_slice_num(PIN_GPIO_BACKLIGHT);
  pwm_set_wrap(slice_num, 255);
  pwm_set_enabled(slice_num, true);

  pwm_set_gpio_level(PIN_GPIO_BACKLIGHT, 50);

  enable_Pico_pins();

  st7565_init();

  st7565_command(CMD_DISPLAY_ON);

  st7565_command(CMD_SET_ALLPTS_NORMAL);

  //st7565_set_brightness(0x18); //too bright
  st7565_set_brightness(0x08);

  clear_screen();
}

void display_data(float temperature, float humidity, float pressure) {
  printf("Displaying temp: %f\n", temperature);

  clear_buffer(buffer);

  char tbuf[20] = {0};
  snprintf(tbuf, sizeof(tbuf), "Temp: %3.2f C", temperature); 
  drawstring(buffer, 0, 0, tbuf);
  snprintf(tbuf, sizeof(tbuf), "Hum:  %3.2f kPa", temperature); 
  drawstring(buffer, 0, 3, tbuf);
  snprintf(tbuf, sizeof(tbuf), "Pres: %3.2f %%", temperature); 
  drawstring(buffer, 0, 6, tbuf);
 
  write_buffer(buffer); 
}


