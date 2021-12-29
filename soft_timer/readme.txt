使用此软件定时器库前置条件：
1.mcu需要有一定的heap空间，供给malloc使用。
2.g_soft_timer_ticks需要放在定时器中断中(g_soft_timer_ticks++),建议1ms
3.while(1)循环中需要不断调用soft_timer_run()函数

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
