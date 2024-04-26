
#include <lcd/fonts.h>
#include <stddef.h>

#include <lcd/font-7x13.xbm>
// xbm files don't end with ;? might be a bug in generator
;

int font_widths[] = {Font7x13_width};
int font_heights[] = {Font7x13_height};
int font_glyph_heights[] = {Font7x13_glyph_h};
int font_glyph_widths[] = {Font7x13_glyph_w};
char* font_datas[] = {Font7x13_bits};


int font_width(size_t font) { return font_widths[font]; }
int font_height(size_t font) {return font_heights[font]; }
int font_glyph_width(size_t font) { return font_glyph_widths[font]; }
int font_glyph_height(size_t font) { return font_glyph_heights[font]; }
const char* font_data(size_t font) { return font_datas[font]; }