#ifndef INCLUDED_SENSOR_READER
#define INCLUDED_SENSOR_READER

#include <sensor.h>
#include <stdbool.h>

void init_sensor_reader();
bool read_sensor_values(sensor_values_t *values);

#endif