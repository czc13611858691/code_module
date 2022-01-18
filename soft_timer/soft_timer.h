#ifndef __soft_timer_H_
#define __soft_timer_H_

#pragma once

#include <stdint.h>
#include "user_common.h"

typedef uint16_t soft_timer_base_t;
typedef void (*soft_timer_cb)(void);

extern soft_timer_base_t g_soft_timer_ticks;

static void SoftTimer_IdleTaskRun(void);

void soft_timer_create(soft_timer_base_t period, soft_timer_cb cb);
void soft_timer_run(void);
void soft_timer_delay_us_init(void);
void soft_timer_delay_us(uint32_t xus);
void SoftTimer_IdleTaskCreate(void (*cb)(void));

#endif
