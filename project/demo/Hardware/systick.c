/*!
    \file  systick.c
    \brief the systick configuration file

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "systick.h"

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* ---------------------------------------------------------------
    TIMER4 Configuration: 
    TIMER4CLK = SystemCoreClock / 12 = 10M
    TIMER4 delay = 1 / 10M * 10 = 1us
    --------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER4);

    timer_deinit(TIMER4);

    /* TIMER4 configuration */
    timer_initpara.prescaler         = 11;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 9;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER4,&timer_initpara);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER4);

    timer_flag_clear(TIMER4, TIMER_FLAG_UP);
}

/*!
    \brief      delay a time in us
    \param[in]  count: count in us
    \param[out] none
    \retval     none
*/
void delay_us(uint32_t count)
{
    timer_enable(TIMER4);
    while(count--)
    {
        while(RESET == timer_flag_get(TIMER4, TIMER_FLAG_UP));
        timer_flag_clear(TIMER4, TIMER_FLAG_UP);
    }
    timer_disable(TIMER4);
}

/*!
    \brief      delay a time in ms
    \param[in]  count: count in ms
    \param[out] none
    \retval     none
*/
void delay_ms(uint32_t count)
{
    while(count--)
    {
        delay_us(1000);
    }
}

/*!
    \brief      disable systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_disable(void)
{
    timer_disable(TIMER4);
}
