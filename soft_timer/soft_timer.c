#include "soft_timer.h"
#include <stdlib.h>
#include <string.h>

typedef struct soft_timer_obj
{
    soft_timer_base_t cnt;
    soft_timer_base_t period;
    soft_timer_cb cb;
    struct soft_timer_obj *next;
} soft_timer_obj_t;

soft_timer_obj_t *g_soft_timer_head = NULL;
soft_timer_base_t g_soft_timer_ticks = 0;
uint32_t g_delay_us_Param = 10;

void soft_timer_create(soft_timer_base_t period, soft_timer_cb cb)
{
    soft_timer_obj_t *obj = NULL;
    obj = (soft_timer_obj_t *)malloc(sizeof(soft_timer_obj_t));
    if (obj == NULL)
    {
        return;
    }
    memset(obj, 0, sizeof(soft_timer_obj_t));

    obj->period = period;
    obj->cb = cb;
    obj->next = g_soft_timer_head;
    g_soft_timer_head = obj;
}

void soft_timer_run(void)
{
    soft_timer_obj_t *obj = g_soft_timer_head;
    for (; obj != NULL; obj = obj->next)
    {
        if ((soft_timer_base_t)(g_soft_timer_ticks - obj->cnt) >= obj->period)
        {
            obj->cnt = g_soft_timer_ticks;
            obj->cb();
        }
    }
}

void soft_timer_delay_us_init(void)
{
	volatile uint32_t delay = 100000 * g_delay_us_Param;
	volatile uint32_t tick_s = 0, tick_e = 0;
	tick_s = g_soft_timer_ticks;
	while (delay--)
		;
	tick_e = g_soft_timer_ticks;
	g_delay_us_Param = (uint32_t)((100.0 / (tick_e - tick_s)) * g_delay_us_Param);
}

void soft_timer_delay_us(uint32_t xus)
{
	volatile uint32_t delay = xus * g_delay_us_Param;
	while (delay--)
		;
}
