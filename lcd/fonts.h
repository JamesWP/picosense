#ifndef FOONTS
#define FOONTS

#include <stddef.h>

int font_width(size_t font);
int font_height(size_t font);
int font_glyph_width(size_t font);
int font_glyph_height(size_t font);
const char* font_data(size_t font);

#endif