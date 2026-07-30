/*
 * Represents a periodically scheduled task managed
 * by the AstraRT cooperative scheduler.
 */

#ifndef TASK_H
#define TASK_H

#include <stdint.h>


typedef void (*TaskFunction)(void);


typedef struct
{
    char *name;

    void (*function)(void);

    uint32_t period_ms;

    uint32_t last_run;

    uint32_t run_count;

    uint32_t execution_time;

} Task;


#endif
