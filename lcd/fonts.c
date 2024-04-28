
#include <lcd/fonts.h>
#include <stddef.h>

#include <lcd/font-7x13.xbm>
// xbm files don't end with ;? might be a bug in generator
;

#include <lcd/font-10x20.xbm>
// xbm files don't end with ;? might be a bug in generator
;

int font_widths[] = {Font7x13_width, Font10x20_width};
int font_heights[] = {Font7x13_height, Font10x20_height};
int font_glyph_heights[] = {Font7x13_glyph_h, Font10x20_glyph_h};
int font_glyph_widths[] = {Font7x13_glyph_w, Font10x20_glyph_w};
char* font_datas[] = {Font7x13_bits, Font10x20_bits};


int font_width(size_t font) { return font_widths[font]; }
int font_height(size_t font) {return font_heights[font]; }
int font_glyph_width(size_t font) { return font_glyph_widths[font]; }
int font_glyph_height(size_t font) { return font_glyph_heights[font]; }
const char* font_data(size_t font) { return font_datas[font]; }