#ifndef INCLUDED_SENSOR
#define INCLUDED_SENSOR

typedef struct sensor_values
{
    float temperature_celcius;
    float humidity_rel;
    float pressure_kPa;
} sensor_values_t;

#endif