#include "main.h"
/* 测试用串口命令字 */
#define ARRAYNUM(arr_nanme) (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define RESET_SIZE          (ARRAYNUM(RESET_BUF) - 1)
#define BRUSH_SIZE          (ARRAYNUM(BRUSH_BUF) - 1)
#define RESTART_SIZE        (ARRAYNUM(RESTART_BUF) - 1)
uint8_t RESET_BUF[] = "reboot";
uint8_t BRUSH_BUF[] = "brush";
uint8_t RESTART_BUF[] = "restart";
/* 算法库需要的全局变量 */
unsigned char FPDATA[TPFS_Work_Size];
/* 调试信息打印 */
void debug_printf(char *time, char *file, int line, char *msg, int32_t value, uint8_t decode)
{
    #ifdef DEBUG_PRINTF
        printf("> [%s]", time), printf("(%s)", file), printf("<%d>", line);
        switch (decode)
        {
            case 0:
                printf(" %s\r\n", msg);
                break;
            case 10:
                printf(" %s: %ld\r\n", msg, value);
                break;
            case 16:
                printf(" %s: 0x%08X\r\n", msg, value);
                break;
            default:
                break;
        }
    #endif
}
/* 字符串比对函数 */
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length) 
{
    while(length--){
        if(*src++ != *dst++){
            return ERROR;
        }
    }
    return SUCCESS;
}
#ifdef USB_DOWNLOAD
    /* 生产测试函数 */
    int GDSWUSBTestMain(uint8_t* fpWorkArea, uint8_t mode);
#endif
/*!
    \brief      main routine
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t ret = 0;

#ifdef USB_DOWNLOAD
    RCU_APB2RST =0xffffffff;
    RCU_APB1RST =0xffffffff;
    RCU_APB2RST =0x00000000;
    RCU_APB1RST =0x00000000;
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000);
    __enable_irq();
#endif
    /* set the priority group */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    /* initilize systick */
    systick_config();
    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 1);
    /* initilize the USART*/
    gd_eval_com_init(EVAL_COM0);
    /* 初始化算法库 */
    ret |= TFPSLib_Init(FPDATA, TPFS_Work_Size);
    debug_printf(__TIME__, __FILE__, __LINE__, "TFPSLib_Init RET", ret & 0xFF, 16);
    /* 初始化传感器 */
    ret |= TFPSLib_CapInit(FPDATA, 0) << 8;
    debug_printf(__TIME__, __FILE__, __LINE__, "TFPSLib_CapInit RET", (ret >> 8) & 0xFF, 16);
    /* 检查授权码 */
    ret |= TFPSLib_CheckLic(FPDATA) << 16;
    debug_printf(__TIME__, __FILE__, __LINE__, "TFPSLib_CheckLic RET", (ret >> 16) & 0xFF, 16);
#ifdef USB_DOWNLOAD
    /* 生产测试 */
    GDSWUSBTestMain(FPDATA, 0);
#endif
    if (ret == 0x000000)
    {
        debug_printf(__TIME__, __FILE__, __LINE__, "Start App ...", 0, 0);
        tfps_app();
        debug_printf(__TIME__, __FILE__, __LINE__, "End App ...", 0, 0);
    }
    else
        debug_printf(__TIME__, __FILE__, __LINE__, "Error Warning ...", 0, 0);
    /* 初始化LED灯对应的IO口 */
    led_init(GREEN), led_init(BLUE), led_init(RED);
    /* 初始化定时器2 */
    nvic_irq_enable(TIMER2_IRQn, 0, 2);
    timer2_config();
    /* 打开各中断 */
    timer_interrupt_enable(TIMER2,TIMER_INT_UP);
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    while(1)
    {
        if (USART_RX_STA & 0x8000)
        {
            usart_interrupt_disable(USART0, USART_INT_RBNE);
            if (SUCCESS == memory_compare(RESET_BUF, USART_RX_BUF, RESET_SIZE))
            {
                debug_printf(__TIME__, __FILE__, __LINE__, "Reboot System ...", 0, 0);
                delay_ms(500);
                __set_FAULTMASK(1);
                NVIC_SystemReset();
            }
            else if (SUCCESS == memory_compare(BRUSH_BUF, USART_RX_BUF, BRUSH_SIZE))
            {
                debug_printf(__TIME__, __FILE__, __LINE__, "Brush Sensor ...", 0, 0);
                TFPSLib_CapInit(FPDATA, 1);
            }
            else if (SUCCESS == memory_compare(RESTART_BUF, USART_RX_BUF, RESTART_SIZE))
            {
                if (ret == 0x000000)
                {
                    debug_printf(__TIME__, __FILE__, __LINE__, "Restart App ...", 0, 0);
                    tfps_app();
                    debug_printf(__TIME__, __FILE__, __LINE__, "End App ...", 0, 0);
                }
                else
                    debug_printf(__TIME__, __FILE__, __LINE__, "Error Warning ...", 0, 0);
            }
            USART_RX_STA = 0;
            usart_interrupt_enable(USART0, USART_INT_RBNE);
        }
    }
}
