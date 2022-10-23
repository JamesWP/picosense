#ifndef INCLUDED_SENSOR_REPORTER
#define INCLUDED_SENSOR_REPORTER

#include <sensor.h>

void init_sensor_reporter();
void report_sensor_values(sensor_values_t *values);

#endif