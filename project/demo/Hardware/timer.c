#include "timer.h"

/**
    \brief      configure the TIMER peripheral for delay
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer2_config(void)
{
    /* ---------------------------------------------------------------
    TIMER2 Configuration: 
    TIMER2CLK = SystemCoreClock / 12000 = 10K
    TIMER2 delay = 1 / 10K * 10000 = 1 s
    --------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);

    timer_deinit(TIMER2);

    /* TIMER2 configuration */
    timer_initpara.prescaler         = 11999;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 9999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);

    /* TIMER2 counter enable */
    timer_enable(TIMER2);
}

/**
  * @brief  This function handles TIMER2 interrupt request.
  * @param  None
  * @retval None
  */
void TIMER2_IRQHandler(void)
{
    if (RESET != timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP))
    {
        timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);
        /* 控制LED灯闪烁 */
        led_toggle(GREEN), led_toggle(BLUE), led_toggle(RED);
    }
}
