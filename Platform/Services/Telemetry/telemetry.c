#include "telemetry.h"
#include "../Protocol/protocol.h"
#include "../../HAL/UART/astra_uart.h"

#include <stdio.h>
#include <string.h>

#define TELEMETRY_BUFFER_SIZE 150

TelemetryData telemetry_data;
static uint32_t bmi_errors = 0;
static uint8_t telemetry_enabled = 0;


void telemetry_init(void)
{
    telemetry_data.temperature = 0;
    telemetry_data.pressure = 0;
    telemetry_data.humidity = 0;

    telemetry_data.accel_x = 0;
    telemetry_data.accel_y = 0;
    telemetry_data.accel_z = 0;
}


void telemetry_send(void)
{
    if(!telemetry_enabled)
    {
        return;
    }

    Packet packet;

    protocol_create(
        &packet,
        PACKET_SENSOR,
        (uint8_t *)&telemetry_data,
        sizeof(TelemetryData)
    );


    UART_Send(
        (uint8_t *)&packet,
        sizeof(Packet)
    );
}

void telemetry_print(void)
{
	char buffer[TELEMETRY_BUFFER_SIZE];

    sprintf(
        buffer,
        "\r\n--- TELEMETRY ---\r\n"
        "TEMP: %.2f C\r\n"
        "PRESSURE: %.2f hPa\r\n"
        "HUMIDITY: %.2f %%\r\n"
        "ACCEL X: %.0f\r\n"
        "ACCEL Y: %.0f\r\n"
        "ACCEL Z: %.0f\r\n",
        telemetry_data.temperature,
		telemetry_data.pressure / 100.0f,
        telemetry_data.humidity,
        telemetry_data.accel_x,
        telemetry_data.accel_y,
        telemetry_data.accel_z
    );

    UART_Send(
        (uint8_t*)buffer,
        strlen(buffer)
    );
}

void telemetry_enable(void)
{
    telemetry_enabled = 1;
}


void telemetry_disable(void)
{
    telemetry_enabled = 0;
}

uint32_t telemetry_get_bmi_errors(void)
{
    return bmi_errors;
}

void telemetry_increment_bmi_error(void)
{
    bmi_errors++;
}
