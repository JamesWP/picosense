
#include <lcd/icons.h>
#include <stddef.h>

#include <lcd/Pressure.xbm>
// xbm files don't end with ;? might be a bug in generator
;

#include <lcd/Temperature.xbm>
// xbm files don't end with ;? might be a bug in generator
;

#include <lcd/Humidity.xbm>
// xbm files don't end with ;? might be a bug in generator
;

int icon_widths[] = {Pressure_width, Temperature_width, Humidity_width};
int icon_heights[] = {Pressure_height, Temperature_height, Humidity_height};
char* icon_datas[] = {Pressure_bits, Temperature_bits, Humidity_bits};

int icon_width(size_t icon) { return icon_widths[icon]; }
int icon_height(size_t icon) {return icon_heights[icon]; }
const char* icon_data(size_t icon) { return icon_datas[icon]; }