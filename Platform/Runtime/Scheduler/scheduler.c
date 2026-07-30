/******************************************************************************
 * @file scheduler.c
 *
 * Cooperative scheduler responsible for periodic task execution,
 * runtime profiling, and task dispatching.
 ******************************************************************************/

#include "scheduler.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define MAX_TASKS 10


static Task *task_list[MAX_TASKS];

static uint8_t task_count = 0;


void scheduler_timer_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;

    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void scheduler_init(void)
{
    task_count = 0;
}

void scheduler_add(Task *task)
{
    if(task_count < MAX_TASKS)
    {
        task_list[task_count] = task;

        task_count++;
    }
}

uint32_t get_cpu_cycles(void)
{
    return DWT->CYCCNT;
}

void scheduler_run(void)
{
    uint32_t current_time = HAL_GetTick();

    for(uint8_t i = 0; i < task_count; i++)
    {
        Task *task = task_list[i];

        if((current_time - task->last_run) >= task->period_ms)
        {
            task->last_run = current_time;

            uint32_t start = get_cpu_cycles();

            task->function();

            uint32_t end = get_cpu_cycles();

            task->execution_time =
                (end - start) / (SystemCoreClock / 1000000);

            task->run_count++;
        }
    }
}

uint8_t scheduler_get_task_count(void)
{
    return task_count;
}


Task* scheduler_get_task(uint8_t index)
{
    if(index < task_count)
    {
        return task_list[index];
    }

    return 0;
}
