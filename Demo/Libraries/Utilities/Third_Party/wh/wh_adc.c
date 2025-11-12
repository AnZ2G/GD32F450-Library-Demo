/*********************************************************************************************************************
* @file				wh_adc.c
* @author			Andreas WH
* @version			3.0
* @Target core		GD32F450ZIT6
* @date				2021-8-16
********************************************************************************************************************/

#include "wh_adc.h"
#include "systick.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC初始化
// @param		ch_gpio			选择ADC通道端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		ch_pin			选择ADC通道引脚：GPIO_PIN_x(x=0..15)
// @param		adc				选择ADC外设：ADCx(x = 0,1,2)
// @param		adc_clk			配置ADC时钟：RCU_ADCx(x = 0,1,2)
// @param		resolution		选择选择通道分辨率：ADC_RESOLUTION_xB(x = 6,8,10,12)
//								(如果同一个模块初始化时设置了不同的分辨率 则第一个初始化的分辨率生效)
// @return		void
// Sample usage:				adc_init(GPIOA, GPIO_PIN_13, ADC0, RCU_GPIOA, RCU_ADC0))//初始化PA13为ADC功能 分辨率为6位
//-------------------------------------------------------------------------------------------------------------------
void adc_init(uint32_t ch_gpio, uint32_t ch_pin, uint32_t adc, rcu_periph_enum gpio_clk, rcu_periph_enum adc_clk)
{
    /* 使能GPIO时钟 */
    rcu_periph_clock_enable(gpio_clk);
    /* 配置通道GPIO */
	gpio_mode_set(ch_gpio, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ch_pin);
    /* 使能ADC时钟 */
    rcu_periph_clock_enable(adc_clk);
    /* 配置ADC时钟来源 */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV6);  
    /* 复位 ADC */
    adc_deinit();
    /* ADC 模式配置 */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    /* ADC 连续功能禁用 */
    adc_special_function_config(adc, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC 扫描模式禁用 */
    adc_special_function_config(adc, ADC_SCAN_MODE, DISABLE);
    /* ADC 数据对齐方式配置 */
    adc_data_alignment_config(adc, ADC_DATAALIGN_RIGHT);
    /* ADC 通道长度配置 */
    adc_channel_length_config(adc, ADC_REGULAR_CHANNEL, 1U);
    
    /* ADC 触发配置 */
    adc_external_trigger_source_config(adc, ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0); 
    adc_external_trigger_config(adc, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    /* 使能ADC */
    adc_enable(adc);
    delay_1ms(1U);
    /* ADC校准和复位校准 */
    adc_calibration_enable(adc);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC转换一次
// @param		adc				选择ADC外设：ADCx(x = 0,1,2)
// @param		ch				选择ADC通道：ADC_CHANNEL_x(x = 0..17)
// @return		uint16			转换的ADC值
// Sample usage:				uint16_t data; data = adc_convert(ADC0, ADC_CHANNEL_0); //获取ADC0通道0转换值
//-------------------------------------------------------------------------------------------------------------------
uint16_t adc_convert(uint32_t ch_gpio, uint32_t ch_pin)
{

    uint32_t adc_periph;
    uint8_t adc_channel;

    if(ch_gpio == GPIOF && ch_pin == GPIO_PIN_5) 
    {
        adc_periph = ADC2;
        adc_channel = ADC_CHANNEL_12;
    }
    else if(ch_gpio == GPIOF && ch_pin == GPIO_PIN_6) 
    {
        adc_periph = ADC2;
        adc_channel = ADC_CHANNEL_4;
    }
    else if(ch_gpio == GPIOF && ch_pin == GPIO_PIN_7) 
    {
        adc_periph = ADC2;
        adc_channel = ADC_CHANNEL_5;
    }
    else if(ch_gpio == GPIOF && ch_pin == GPIO_PIN_8) 
    {
        adc_periph = ADC2;
        adc_channel = ADC_CHANNEL_6;
    }
    else if(ch_gpio == GPIOF && ch_pin == GPIO_PIN_9) 
    {
        adc_periph = ADC2;
        adc_channel = ADC_CHANNEL_7;
    }
    else if(ch_gpio == GPIOF && ch_pin == GPIO_PIN_10) 
    {
        adc_periph = ADC2;
        adc_channel = ADC_CHANNEL_8;
    }

    /* 配置ADC通道和转换时间 */
    adc_regular_channel_config(adc_periph, 0U, adc_channel, ADC_SAMPLETIME_15);
    /* 使能ADC软件触发 */
    adc_software_trigger_enable(adc_periph, ADC_REGULAR_CHANNEL);

    /* 等待ADC转换完成 */
    while(!adc_flag_get(adc_periph, ADC_FLAG_EOC));
    /* 清除转换完成标志位 */
    adc_flag_clear(adc_periph,ADC_FLAG_EOC);
    /* 返回ADC的值 */
    return(adc_regular_data_read(adc_periph));    
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC均值滤波
// @param		adc				选择ADC外设：ADCx(x = 0,1,2)
// @param		ch				选择ADC通道：ADC_CHANNEL_x(x = 0..17)
// @param		count			均值滤波次数
// @return		uint16			转换的ADC值
// Sample usage:				uint16_t data; data = adc_mean_filter(ADC0, ADC_CHANNEL_0,5);//采集5次 然后返回平均值
//-------------------------------------------------------------------------------------------------------------------
uint16_t adc_mean_filter(uint32_t ch_gpio, uint32_t ch_pin, uint8_t count)
{
	uint8_t i;
	uint16_t sum;

	sum = 0;
	for(i=0; i<count; i++)
	{
		sum += adc_convert(ch_gpio, ch_pin);
	}

	return sum/count;
}
