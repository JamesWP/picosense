#include <lcd/display.h>
#ifdef PICO_SDK
#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <lcd/stlcd.h>
#endif
#include <stdio.h>
#include <lcd/glcd.h>
#include <lcd/pins.h>

uint8_t buffer[128 * 64 / 8] = {0};

void init_display(void)
{
  printf("Initializing display\n");

#ifdef ST7565
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

  // st7565_set_brightness(0x18); //too bright
  st7565_set_brightness(0x08);
#endif

  clear_screen();
}

int val_row(uint8_t *buffer, int xloc, int y, float value, int icon)
{
  int px = (int)(value * 10) % 10;
  int x = (int)(value);

  char text[10];

  sprintf(text, "%3d", x);
  drawstring(buffer, 1, xloc, y, text);

  sprintf(text, "%d", px);
  drawstring(buffer, 0, xloc + 33, y + 5, text);

  drawrect(buffer, xloc + 30, y + 15, 1, 1, 1);

  drawicon(buffer, icon, xloc + 105 - 55, y);
}

void display_data(float temperature, float humidity, float pressure)
{
  printf("Displaying temp: %f\n", temperature);

  clear_buffer(buffer);

  val_row(buffer, 5, 7, pressure, 0);
  val_row(buffer, 5, 65-25, temperature, 1);
  // val_row(buffer, 35, 47, humidity, 0);

  write_buffer(buffer);
}

void display_status(bool wifi, bool mqtt, bool publish, const char *code, unsigned int failures)
{
  drawchar(buffer, 0, 127-(7*3), 0, (char)(wifi ? 'O' : 'X'));
  drawchar(buffer, 0, 127-(7*2), 0, (char)(mqtt ? 'O' : 'X'));
  drawchar(buffer, 0, 127-(7*1), 0, (char)(publish ? 'O' : 'X'));
  drawstring(buffer, 0, 127-(7*2), 13, (unsigned char*)code);

  char numbuff[4];
  snprintf(numbuff, sizeof(numbuff), "%3d", failures);
  drawstring(buffer, 0, 127-(7*3), 13*2, numbuff);
  
  write_buffer(buffer);
}

bool display_getpixel(uint32_t x, uint32_t y)
{
  return getpixel(buffer, x, y);
}
