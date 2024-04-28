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
*/

#include "glcd.h"
#include <lcd/fonts.h>
#include <lcd/icons.h>
#include <stdbool.h>

// the most basic function, set a single pixel
void setpixel(uint8_t *buff, uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color)
    buff[x + (y / 8) * 128] |= _BV(7 - (y % 8));
  else
    buff[x + (y / 8) * 128] &= ~_BV(7 - (y % 8));
}

bool  getpixel(uint8_t *buff, uint8_t x, uint8_t y){
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return false;

  return buff[x + (y / 8) * 128] & _BV(7 - (y % 8));
}

void drawbitmap(uint8_t *buff, uint8_t x, uint8_t y, const uint8_t *bitmap,
                uint8_t w, uint8_t h, uint8_t color) {
  for (uint8_t j = 0; j < h; j++) {
    for (uint8_t i = 0; i < w; i++) {
      if (bitmap[i + (j / 8) * w] & _BV(j % 8)) {
        setpixel(buff, x + i, y + j, color);
      }
    }
  }
}

void drawstring(uint8_t *buff, size_t font, uint8_t x, uint8_t line, uint8_t *c) {
  while (c[0] != 0) {
    drawchar(buff, font, x, line, c[0]);
    c++;
    x += font_glyph_width(font); // 6 pixels wide
    if (x + font_glyph_width(font) >= LCDWIDTH) {
      x = 0; // ran out of this line
      line+= font_glyph_height(font);
    }
    if (line >= LCDHEIGHT)
      return; // ran out of space :(
  }
}

void draw_image_section(uint8_t *buffer, uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *source, uint8_t source_offset_x, uint8_t source_offset_y, uint8_t source_w, uint8_t source_h);

void drawchar(uint8_t *buff, size_t font, uint8_t x, uint8_t y, uint8_t c) {
  int glyphs_per_row = font_width(font) / font_glyph_width(font);
  int glyph_row = c / glyphs_per_row;
  int glyph_col = c % glyphs_per_row;
  int glyph_x = glyph_col * font_glyph_width(font);
  int glyph_y = glyph_row * font_glyph_height(font);
  int glyph_w = font_glyph_width(font);
  int glyph_h = font_glyph_height(font);

  const uint8_t *data = font_data(font);

  draw_image_section(buff, x, y, glyph_w, glyph_h, data, glyph_x, glyph_y, font_width(font), font_height(font));
}

void drawicon(uint8_t *buffer, size_t icon, uint8_t x, uint8_t y) {
  int icon_w = icon_width(icon);
  int icon_h = icon_height(icon);
  int image_offset_x = 0;
  int image_offset_y = 0;
  int image_w = icon_w;
  int image_h = icon_h;

  const uint8_t *data = icon_data(icon);

  draw_image_section(buffer, x, y, icon_w, icon_h, data, image_offset_x, image_offset_y, image_w, image_h);
}

void draw_image_section(uint8_t *buff, uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *source, uint8_t source_offset_x, uint8_t source_offset_y, uint8_t source_w, uint8_t source_h) {
  int bytes_per_row = (source_w+7) >> 3;
  int image_col_byte_start = source_offset_x >> 3;
  int image_col_byte_end = (source_offset_x+w+7) >> 3;
  int image_row_byte_start = source_offset_y * bytes_per_row;
  int image_row_byte_end = (source_offset_y+h) * bytes_per_row;

  //printf("Testing: image width, height: %d, %d\n", source_w, source_h);
  //printf("Testing: bpr: %d\n", bytes_per_row);

  //printf("Testing: x,y: %d, %d\n", source_offset_x, source_offset_y);
  //printf("Testing: col start,end: %d, %d\n", image_col_byte_start, image_col_byte_end);
  //printf("Testing: row start,end: %d, %d\n", image_row_byte_start, image_row_byte_end);

  int starting_x = x;

  for (int image_row_byte = image_row_byte_start; image_row_byte < image_row_byte_end; image_row_byte+=bytes_per_row) {
    for(int image_col_byte = image_col_byte_start; image_col_byte < image_col_byte_end; image_col_byte++) {
      int byte = image_row_byte + image_col_byte;
      int byte_pix_col = image_col_byte << 3;
      char colors = source[byte];
      for(int i = byte_pix_col; i < byte_pix_col+8; i++, colors >>= 1) {
        if (i < source_offset_x) continue;
        if (i >= source_offset_x+w) continue;
        int color = colors&1;
        setpixel(buff, x, y, color);
        x++;
      }
    }
    y++;
    x = starting_x;
  }
}

// the most basic function, clear a single pixel
void clearpixel(uint8_t *buff, uint8_t x, uint8_t y) {
  // x is which column
  buff[x + (y / 8) * 128] &= ~_BV(7 - (y % 8));
}

// bresenham's algorithm - thx wikpedia
void drawline(uint8_t *buff, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
              uint8_t color) {

  uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  uint8_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 < x1; x0++) {
    if (steep) {
      setpixel(buff, y0, x0, color);
    } else {
      setpixel(buff, x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// filled rectangle
void fillrect(uint8_t *buff, uint8_t x, uint8_t y, uint8_t w, uint8_t h,
              uint8_t color) {

  // stupidest version - just pixels - but fast with internal buffer!
  for (uint8_t i = x; i < x + w; i++) {
    for (uint8_t j = y; j < y + h; j++) {
      setpixel(buff, i, j, color);
    }
  }
}

// draw a rectangle
void drawrect(uint8_t *buff, uint8_t x, uint8_t y, uint8_t w, uint8_t h,
              uint8_t color) {
  // stupidest version - just pixels - but fast with internal buffer!
  for (uint8_t i = x; i < x + w; i++) {
    setpixel(buff, i, y, color);
    setpixel(buff, i, y + h - 1, color);
  }
  for (uint8_t i = y; i < y + h; i++) {
    setpixel(buff, x, i, color);
    setpixel(buff, x + w - 1, i, color);
  }
}

// draw a circle
void drawcircle(uint8_t *buff, uint8_t x0, uint8_t y0, uint8_t r,
                uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  setpixel(buff, x0, y0 + r, color);
  setpixel(buff, x0, y0 - r, color);
  setpixel(buff, x0 + r, y0, color);
  setpixel(buff, x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    setpixel(buff, x0 + x, y0 + y, color);
    setpixel(buff, x0 - x, y0 + y, color);
    setpixel(buff, x0 + x, y0 - y, color);
    setpixel(buff, x0 - x, y0 - y, color);

    setpixel(buff, x0 + y, y0 + x, color);
    setpixel(buff, x0 - y, y0 + x, color);
    setpixel(buff, x0 + y, y0 - x, color);
    setpixel(buff, x0 - y, y0 - x, color);
  }
}

// draw and fill a circle
void fillcircle(uint8_t *buff, uint8_t x0, uint8_t y0, uint8_t r,
                uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  for (uint8_t i = y0 - r; i <= y0 + r; i++) {
    setpixel(buff, x0, i, color);
  }

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    for (uint8_t i = y0 - y; i <= y0 + y; i++) {
      setpixel(buff, x0 + x, i, color);
      setpixel(buff, x0 - x, i, color);
    }
    for (uint8_t i = y0 - x; i <= y0 + x; i++) {
      setpixel(buff, x0 + y, i, color);
      setpixel(buff, x0 - y, i, color);
    }
  }
}

// clear everything
void clear_buffer(uint8_t *buff) { memset(buff, 0, 1024); }
