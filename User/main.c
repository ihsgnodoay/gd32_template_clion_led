#include "systick.h"
#include <stdio.h>
#include "usart.h"
#include "gd32f4xx.h"
#include "bsp_basic_timer.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "i2c.h"


void usart_receive(uint8_t *data, uint32_t len)
{
        printf("recv: %s\r\n", data);
}

void adc_config()
{
        /* GPIO 配置 */
        rcu_periph_clock_enable(RCU_GPIOA);
        gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);


        /* ADC 配置 */
        rcu_periph_clock_enable(RCU_ADC0);
        adc_deinit();

        /* 主频 */
        adc_clock_config(ADC_ADCCK_PCLK2_DIV8);

        /*ADC 基本配置*/
        adc_resolution_config(ADC0, ADC_RESOLUTION_12B);        // 分辨率
        adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);        // 数据对齐方式，右对齐

        /* ADC 模式配置 */
        adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);        // 扫描模式
        adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);// 连续模式

        /* ADC 规则配置 */
        // 电位器
        adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 2);
        adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_16, ADC_SAMPLETIME_15);        // 温度
        adc_routine_channel_config(ADC0, 1, ADC_CHANNEL_0, ADC_SAMPLETIME_15);        // 电位器
        adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);

        /* ADC 需要开启 DMA */
        adc_dma_mode_enable(ADC0);
        adc_dma_request_after_last_enable(ADC0);

        /* 启动ADC */
        adc_enable(ADC0);
        delay_1ms(1);

        /* ADC 校准 */
        adc_calibration_enable(ADC0);
        // 触发ADC进行采样：外部触发/内部触发(软触发)
        adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
}

void adc_get_vol()
{
        /* 触发ADC进行采样：外部中断（EXTI）触发，内部触发（软中断） */
        adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);

        /* 等待ADC采样完成，获取ADC状态值 */
        while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET);

        adc_flag_clear(ADC0, ADC_FLAG_EOC);
        uint16_t value = adc_routine_data_read(ADC0);

        float v = 3.3 * value / 4096;
        printf("vol: %.2f & value: %d\r\n", v, value);
}

void adc_get_temperature()
{
        /* 触发ADC进行采样：外部中断（EXTI）触发，内部触发（软中断） */
        adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);

        /* 等待ADC采样完成，获取ADC状态值 */
        while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET);

        adc_flag_clear(ADC0, ADC_FLAG_EOC);
        uint16_t value = adc_routine_data_read(ADC0);
        float temperature = (1.4 - value) / 4.4 + 25;
        printf("value: %d & temp: %f \r\n", value, temperature);
}

#define BUFF_LENGTH 2

uint16_t buff[BUFF_LENGTH] = {0};

static void dma_config()
{
        uint32_t dmax = DMA1;
        int32_t dmax_rcu = RCU_DMA1;
        uint32_t dmax_ch = DMA_CH0;
        uint32_t damx_sub = DMA_SUBPERI0;

        rcu_periph_clock_enable(dmax_rcu);
        dma_deinit(DMA1, dmax_ch);

        /* DMA 配置 */
        dma_single_data_parameter_struct dsdps;
        dma_single_data_para_struct_init(&dsdps);
        dsdps.direction = DMA_PERIPH_TO_MEMORY;

        /* 内存：dst */
        dsdps.memory0_addr = (uint32_t) buff;
        dsdps.memory_inc = DMA_MEMORY_INCREASE_ENABLE;

        /* 外设：src */
        dsdps.periph_addr = (uint32_t) (&ADC_RDATA(ADC0));
        dsdps.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dsdps.number = BUFF_LENGTH;
        dsdps.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
        dsdps.priority = DMA_PRIORITY_ULTRA_HIGH;
        dma_single_data_mode_init(dmax, dmax_ch, &dsdps);

        /* 配置 dma 子连接 */
        dma_channel_subperipheral_select(dmax, dmax_ch, damx_sub);

        // 开启DMA循环查询
        dma_circulation_enable(dmax, dmax_ch);

        // 默认开启接收
        dma_flag_clear(dmax, dmax_ch, DMA_FLAG_FTF);
        dma_channel_enable(dmax, dmax_ch);
}

static void adc_dma_get()
{
        /* 温度 */
        uint16_t value = buff[0];
        float temperature = (1.4 - 3.3 * value / 4096) / 4.4 + 25;
        printf("\r\n");
        printf("value: %d & temp: %.2f \r\n", value, temperature);

        printf("==================================================");

        /* 电位器 */
        value = buff[1];
        float v = 3.3 * value / 4096;
        printf("\r\n");
        printf("vol: %.2f & value: %d\r\n", v, value);
}

void fonts_demo1()
{
        lv_obj_t *label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "HELLO WORLD");
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        // 创建样式
        static lv_style_t style;
        lv_style_init(&style);
        lv_style_set_text_font(&style, &lv_font_montserrat_28);
        //lv_style_set_width(&style, 300);
        lv_style_set_text_color(&style, lv_palette_main(LV_PALETTE_RED));

        //将样式绑定到显示对象
        lv_obj_add_style(label, &style, 0);
}

int main(void)
{
        nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
        // 系统时钟初始化
        systick_config();
        basic_timer_config(24000, 1 * 10);
        usart_init(115200);
        adc_config();
        dma_config();
        I2C_init();
        ST7789_Init();

        lv_init();
        lv_port_disp_init();
        lv_port_indev_init();
        fonts_demo1();
        adc_dma_get();

        fonts_demo1();

        while (1) {
                //adc_get_vol();
                //adc_get_temperature();
                lv_timer_handler();
                //delay_1ms(1000);
        }
}
