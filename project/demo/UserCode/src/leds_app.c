#include "leds_app.h"

static rcu_periph_enum GPIO_CLK[LEDn] = {GREEN_GPIO_CLK, BLUE_GPIO_CLK, RED_GPIO_CLK};
static uint32_t GPIO_PORT[LEDn] = {GREEN_GPIO_PORT, BLUE_GPIO_PORT, RED_GPIO_PORT};
static uint32_t GPIO_PIN[LEDn] = {GREEN_PIN, BLUE_PIN, RED_PIN};

void led_init(led_typedef_enum lednum)
{
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    /* configure led GPIO port */ 
    gpio_init(GPIO_PORT[lednum], GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN[lednum]);
    /* GPIO bit clear register */
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

void led_toggle(led_typedef_enum lednum)
{
    gpio_bit_write(GPIO_PORT[lednum], GPIO_PIN[lednum], (bit_status)(1-gpio_input_bit_get(GPIO_PORT[lednum], GPIO_PIN[lednum])));
}
