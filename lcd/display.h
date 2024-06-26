#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdbool.h>
#include <stdint.h>

void init_display(void);
void display_data(float, float, float);
void display_status(bool, bool, bool, const char*, unsigned int);
bool display_getpixel(uint32_t x, uint32_t y);

#endif
