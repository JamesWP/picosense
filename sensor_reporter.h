#ifndef INCLUDED_SENSOR_REPORTER
#define INCLUDED_SENSOR_REPORTER

#include <sensor.h>
#include <stdbool.h>

typedef struct reporting_status {
    bool is_wifi_connected;
    bool is_mqtt_connected;
    bool is_publish_success;

    char wifi_state[3]; // two chars plus NULL
} reporting_status_t;

void init_sensor_reporter();
void report_sensor_values(sensor_values_t *values, reporting_status_t *status);

#endif