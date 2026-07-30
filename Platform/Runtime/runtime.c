#include "runtime.h"

#include "Scheduler/scheduler.h"

#include "../Application/app_tasks.h"

#include "../Drivers/BME280/bme280.h"

#include "../Services/Telemetry/telemetry.h"


void runtime_init(void)
{
    /* Initialize platform services */
    scheduler_init();
    BME280_Init();
    telemetry_init();

    /* Register application tasks */
    scheduler_add(&ledTask);
    scheduler_add(&uartTask);
    scheduler_add(&sensorTask);
    scheduler_add(&telemetryTask);
    scheduler_add(&commandTask);
    scheduler_add(&diagnosticsTask);
}
