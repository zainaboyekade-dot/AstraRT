/*
 * AstraRT application task definitions.
 *
 * Contains periodic tasks responsible for
 * sensor acquisition, telemetry, commands,
 * and system services.
 */

#include "app_tasks.h"

#include "main.h"

#include "../Runtime/Scheduler/scheduler.h"
#include "../Runtime/Task/task.h"

#include "../Drivers/BME280/bme280.h"
#include "../Drivers/BMI160/bmi160.h"

#include "../HAL/UART/astra_uart.h"

#include "../Services/Command/command.h"
#include "../Services/Telemetry/telemetry.h"

#include <string.h>

#define CPU_FREQ_MHZ 180U

/*
 * Sends an unsigned integer over UART without
 * relying on the standard printf library.
 */
void print_number(uint32_t number)
{
    char buffer[10];
    int i = 0;

    if(number == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        while(number > 0)
        {
            buffer[i++] = (number % 10) + '0';
            number /= 10;
        }
    }


    for(int j = 0; j < i/2; j++)
    {
        char temp = buffer[j];
        buffer[j] = buffer[i-j-1];
        buffer[i-j-1] = temp;
    }

    UART_Send((uint8_t*)buffer, i);
}


void led_task(void)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}


Task ledTask =
{
    .name = "LED Task",
    .function = led_task,
    .period_ms = 500,
    .last_run = 0,
	.run_count = 0
};

void uart_task(void)
{
    // UART handled by command and telemetry services
}

Task uartTask =
{
    .name = "UART Task",
    .function = uart_task,
    .period_ms = 1000,
    .last_run = 0,
	.run_count = 0
};


void sensor_task(void)
{
    float temperature;
    float pressure;
    float humidity;

    float accel_x;
    float accel_y;
    float accel_z;

    /* Read environmental sensors */
    BME280_ReadTemperature(&temperature);

    BME280_ReadPressure(&pressure);

    BME280_ReadHumidity(&humidity);

    /* Read IMU */
    if(BMI160_ReadAccel(
        &accel_x,
        &accel_y,
        &accel_z
    ) != HAL_OK)
    {
    	telemetry_increment_bmi_error();
    }

    /* Update shared telemetry */
    telemetry_data.temperature = temperature;
    telemetry_data.pressure = pressure;
    telemetry_data.humidity = humidity;

    telemetry_data.accel_x = accel_x;
    telemetry_data.accel_y = accel_y;
    telemetry_data.accel_z = accel_z;
}

Task sensorTask =
{
    .name = "Sensor Task",
    .function = sensor_task,
    .period_ms = 200,
    .last_run = 0,
	.run_count = 0
};

void telemetry_task(void)
{
    telemetry_send();
}

Task telemetryTask =
{
    .name = "Telemetry Task",
    .function = telemetry_task,
	.period_ms = 5000,
    .last_run = 0,
    .run_count = 0
};

/* Assemble UART characters into a command string. */
void command_task(void)
{
    static char command[32];
    static uint8_t index = 0;

    uint8_t ch;

    if(UART_Receive(&ch) == HAL_OK)
    {
        if(ch == '\r' || ch == '\n')
        {
            command[index] = '\0';

            command_process(command);

            index = 0;
        }
        else if(index < sizeof(command) - 1)
        {
            command[index++] = ch;
        }
    }
}

Task commandTask =
{
    .name = "Command Task",
    .function = command_task,
    .period_ms = 20,
    .last_run = 0,
    .run_count = 0
};

void print_task_stats(void)
{
	UART_Send((uint8_t*)"\r\n==== SYSTEM METRICS ====\r\n", 29);

    uint8_t count = scheduler_get_task_count();

    for(uint8_t i = 0; i < count; i++)
    {
        Task *task = scheduler_get_task(i);

        UART_Send((uint8_t*)"\r\n[TASK] ", 9);

        UART_Send((uint8_t*)task->name, strlen(task->name));

        UART_Send((uint8_t*)"\r\nExecutions: ", 14);

        print_number(task->run_count);

        UART_Send((uint8_t*)"\r\nPeriod: ", 10);

        print_number(task->period_ms);

        UART_Send((uint8_t*)" ms\r\nRuntime: ", 15);

        uint32_t runtime_us = task->execution_time / CPU_FREQ_MHZ;

        print_number(runtime_us);

        UART_Send((uint8_t*)" us\r\n\r\n", 7);
    }
}

void diagnostics_task(void)
{
    /* Reserved for future system diagnostics. */
}

Task diagnosticsTask =
{
    .name = "Diagnostics Task",
    .function = diagnostics_task,
    .period_ms = 5000,
    .last_run = 0,
    .run_count = 0
};
