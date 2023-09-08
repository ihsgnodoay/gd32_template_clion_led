#include "key.h"
#include "usart.h"

void key_init(void)
{
        /* enable the led clock */
        rcu_periph_clock_enable(KEYL_RCU);
        /* configure led GPIO port */
        gpio_mode_set(KEYL_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEYL_PIN);
        gpio_output_options_set(KEYL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYL_PIN);
}


void key_get_value(uint32_t port, uint32_t pin)
{
        if (RESET == gpio_input_bit_get(port, pin)) {
                delay_1ms(10); // �ӳ�����

                /* check whether the button is pressed */
                if (RESET == gpio_input_bit_get(port, pin)) {
                        /* ִ�а������µĶ��� */
                        printf("currect port is %d,pin is %d\n", port, pin);

                        while (RESET == gpio_input_bit_get(port, pin)); // �ȴ������ɿ�
                }
        };
}
