使用此软件定时器库前置条件：
1.mcu需要有一定的heap空间，供给malloc使用。
2.g_soft_timer_ticks需要放在定时器中断中(g_soft_timer_ticks++),建议1ms
3.while(1)循环中需要不断调用soft_timer_run()函数

***注意:当此软件定时器用于8位单片机可能出现不规则错误，
参考文章如下: --      https://www.cxyzjd.com/article/weixin_42952614/109251285
解决方案:
判断条件更换为
#define OVER_FLOW_VAL_DET 60000U // soft_timer_base_t--uint16_t
if (((soft_timer_base_t)(g_soft_timer_ticks - obj->cnt) >= obj->period) && ((soft_timer_base_t)(g_soft_timer_ticks - obj->cnt) <= OVER_FLOW_VAL_DET))

使用例子:
	soft_timer_create(100, btn_ctrl_led_and_lin_sig);
	soft_timer_create(50, adc_run);
	soft_timer_create(100, hall_signal);
	soft_timer_create(TICKS_INTERVAL, button_ticks);
  while(1)
  {
    soft_timer_run();
  }
  
  
  
  .../
  timer_isr()//定时器中断函数
  {
    g_soft_timer_ticks++;
  }
