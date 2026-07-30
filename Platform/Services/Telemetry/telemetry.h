#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>


typedef struct
{
    float temperature;
    float pressure;
    float humidity;

    float accel_x;
    float accel_y;
    float accel_z;

} TelemetryData;


extern TelemetryData telemetry_data;


void telemetry_init(void);

void telemetry_send(void);

void telemetry_print(void);

void telemetry_enable(void);

void telemetry_disable(void);


uint32_t telemetry_get_bmi_errors(void);

void telemetry_increment_bmi_error(void);


#endif
