#include <lcd/glcd.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdbool.h>

uint8_t buffer[128 * 64 / 8] = {0};

uint8_t png_buffer[128 * 64 * 3 * 4] = {0};

// the most basic function, set a single pixel
bool getpixel(uint8_t *buff, uint32_t x, uint32_t y) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return false;

  return buff[x + (y / 8) * 128] & _BV(7 - (y % 8));
}

void setpng_pixel(uint8_t *buff, uint32_t x, uint32_t y) {
  x *= 2;
  y *= 2;
  for (uint32_t x_ = x; x_ < x + 2; x_++) {
    for (uint32_t y_ = y; y_ < y + 2; y_++) {
      uint32_t pix = x_ * 3 + y_ * 3 * 128 * 2;
      buff[pix] = 255;
      buff[pix + 1] = 255;
      buff[pix + 2] = 255;
    }
  }
}

void convert(uint8_t *buff, uint8_t *png_buff) {
  for (int y = 0; y < 64; y++) {
    for (int x = 0; x < 128; x++) {
      if (getpixel(buff, x, y)) {
        setpng_pixel(png_buff, x, y);
      }
    }
  }
}

int main() {

  //drawline(buffer, 10, 10, 76, 23, 1);
  drawstring(buffer, 1, 0, 0, "HELLO WORLD! Caroline is the best wife");

  convert(buffer, png_buffer);
  const char *file = "output.png";
  stbi_write_png(file, 128 * 2, 64 * 2, 3, png_buffer, 128 * 2 * 3);
}