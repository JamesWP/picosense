#ifndef GLCD
#define GLCD

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define LCDWIDTH 128
#define LCDHEIGHT 64

#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif

#define swap(a, b)                                                             \
  {                                                                            \
    uint8_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
void setpixel(uint8_t *buff, uint8_t x, uint8_t y, uint8_t color);

void drawrect(uint8_t *buff, uint8_t x, uint8_t y, uint8_t w, uint8_t h,
              uint8_t color);
void drawline(uint8_t *buff, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
              uint8_t color);

void drawbitmap(uint8_t *buff, uint8_t x, uint8_t y, const uint8_t *bitmap,
                uint8_t w, uint8_t h, uint8_t color);
void drawicon(uint8_t *buff, size_t icon, uint8_t x, uint8_t y);
void drawchar(uint8_t *buff, size_t font, uint8_t x, uint8_t line, uint8_t c);
void drawstring(uint8_t *buff, size_t font, uint8_t x, uint8_t line, uint8_t *c);

void drawcircle(uint8_t *buff, uint8_t x0, uint8_t y0, uint8_t r,
                uint8_t color);

void fillrect(uint8_t *buff, uint8_t x, uint8_t y, uint8_t w, uint8_t h,
              uint8_t color);
bool getpixel(uint8_t *buff, uint8_t x, uint8_t y);

#endif