/*
 * AstraRT Command Service
 *
 * Provides UART-based command processing
 * for system monitoring, telemetry control,
 * diagnostics, and runtime management.
 */

#include "command.h"
#include "../../HAL/UART/astra_uart.h"
#include "../../Application/app_tasks.h"
#include "../../Drivers/BME280/bme280.h"
#include "../../Drivers/BMI160/bmi160.h"
#include "../Telemetry/telemetry.h"
#include "../Memory/memory.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

void command_uptime(void)
{
    char buffer[50];

    uint32_t uptime = HAL_GetTick();

    sprintf(
        buffer,
        "\r\nSYSTEM UPTIME\r\n%lu ms\r\n",
        uptime
    );

    UART_Send(
        (uint8_t*)buffer,
        strlen(buffer)
    );
}

void command_process(char *command)
{

	command[strcspn(command, "\r\n")] = 0;

	if(command == NULL)
	{
	    return;
	}

	command[strcspn(command, "\r\n")] = 0;

    if(strcmp(command, "help") == 0)
    {
        char message[] =
            "\r\nAvailable Commands\r\n"
        		"help\r\n"
        		"task_stats\r\n"
        		"metrics\r\n"
        		"sensor_status\r\n"
        		"sensor_health\r\n"
        		"sensor_errors\r\n"
        		"telemetry_debug\r\n"
        		"telemetry_on\r\n"
        		"telemetry_off\r\n"
        		"system_status\r\n"
        		"uptime\r\n"
        		"version\r\n"
        		"reboot\r\n"
        		"calibrate\r\n"
        		"memory_status\r\n";

        UART_Send(
            (uint8_t*)message,
            strlen(message)
        );
    }

    else if(strcmp(command, "telemetry_status") == 0)
    {
        telemetry_print();
    }

    else if(strcmp(command, "telemetry_debug") == 0)
    {
        telemetry_print();
    }

    else if(strcmp(command, "telemetry_on") == 0)
    {
        telemetry_enable();

        UART_Send(
            (uint8_t*)"Telemetry ENABLED\r\n",
            20
        );
    }


    else if(strcmp(command, "telemetry_off") == 0)
    {
        telemetry_disable();

        UART_Send(
            (uint8_t*)"Telemetry DISABLED\r\n",
            21
        );
    }

    else if(strcmp(command, "metrics") == 0)
    {
        print_task_stats();
    }

    else if(strcmp(command, "task_stats") == 0)
    {
        print_task_stats();
    }


    else if(strcmp(command, "sensor_status") == 0)
    {
        float temperature;
        float pressure;
        float humidity;


        BME280_ReadTemperature(&temperature);

        BME280_ReadPressure(&pressure);

        BME280_ReadHumidity(&humidity);



        char buffer[150];


        sprintf(
            buffer,
            "\r\nBME280 STATUS\r\n"
            "Temperature: %.2f C\r\n"
            "Pressure: %.2f hPa\r\n"
            "Humidity: %.2f %%\r\n",
            temperature,
            pressure / 100.0f,
            humidity
        );


        UART_Send(
            (uint8_t*)buffer,
            strlen(buffer)
        );
    }

    else if(strcmp(command, "sensor_health") == 0)
    {
        float temp;
        float accel_x;
        float accel_y;
        float accel_z;

        BME280_ReadTemperature(&temp);

        HAL_StatusTypeDef bmi_status;

        bmi_status = BMI160_ReadAccel(
            &accel_x,
            &accel_y,
            &accel_z
        );


        char buffer[150];

        sprintf(
            buffer,
            "\r\nSENSOR HEALTH\r\n"
            "BME280: OK\r\n"
            "BMI160: %s\r\n"
            "Telemetry: OK\r\n",
            (bmi_status == HAL_OK) ? "OK" : "FAIL"
        );


        UART_Send(
            (uint8_t*)buffer,
            strlen(buffer)
        );
    }

    else if(strcmp(command, "sensor_errors") == 0)
    {
        char buffer[120];

        sprintf(
            buffer,
            "\r\n==== SENSOR ERRORS ====\r\n"
            "BMI160 Errors: %lu\r\n",
            telemetry_get_bmi_errors()
        );

        UART_Send(
            (uint8_t*)buffer,
            strlen(buffer)
        );
    }

    else if(strcmp(command, "system_status") == 0)
    {
        char message[] =
            "\r\nSYSTEM STATUS\r\n"
            "Runtime: RUNNING\r\n"
            "Tasks: ACTIVE\r\n"
        	"Sensors: BME280 + BMI160 ONLINE\r\n"
            "Telemetry: ACTIVE\r\n";

        UART_Send(
            (uint8_t*)message,
            strlen(message)
        );
    }

    else if(strcmp(command, "uptime") == 0)
    {
        command_uptime();
    }

    else if(strcmp(command, "version") == 0)
    {
        char message[] =
            "\r\nASTRA RT\r\n"
            "Firmware Version: 1.0\r\n"
        	"Board: STM32F446RE Nucleo\r\n"
            "Sensors: BME280 + BMI160\r\n";

        UART_Send(
            (uint8_t*)message,
            strlen(message)
        );
    }

    else if(strcmp(command, "reboot") == 0)
    {
        char message[] =
            "\r\nRebooting...\r\n";

        UART_Send(
            (uint8_t*)message,
            strlen(message)
        );


        HAL_Delay(100);


        HAL_NVIC_SystemReset();
    }

    else if(strcmp(command, "calibrate") == 0)
    {
        char message[] =
        	"\r\nCALIBRATION STATUS\r\n"
        	"IMU Calibration: NOT IMPLEMENTED\r\n";

        UART_Send(
            (uint8_t*)message,
            strlen(message)
        );
    }

    else if(strcmp(command, "memory_status") == 0)
    {
        memory_print_status();
    }

    else
    {
        UART_Send(
            (uint8_t*)"Unknown command\r\n",
            18
        );
    }

}
