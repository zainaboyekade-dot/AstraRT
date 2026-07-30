#ifndef APP_TASKS_H
#define APP_TASKS_H

#include "../Runtime/Task/task.h"


void led_task(void);

void uart_task(void);

void sensor_task(void);

void telemetry_task(void);

void command_task(void);

void print_task_stats(void);

void diagnostics_task(void);

extern Task ledTask;

extern Task uartTask;

extern Task sensorTask;

extern Task telemetryTask;

extern Task commandTask;

extern Task diagnosticsTask;

#endif
