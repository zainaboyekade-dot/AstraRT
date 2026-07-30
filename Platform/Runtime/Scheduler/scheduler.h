#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../Task/task.h"
#include <stdint.h>

void scheduler_init(void);

void scheduler_add(Task *task);

void scheduler_run(void);

void scheduler_timer_init(void);


uint8_t scheduler_get_task_count(void);

Task* scheduler_get_task(uint8_t index);

#endif
