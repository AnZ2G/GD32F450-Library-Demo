/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				camera
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.3 or MDK 5.28
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "zf_camera.h"

#define DCI_DR_ADDRESS     (0x50050028U)
//-------------------------------------------------------------------------------------------------------------------
// @brief		摄像头DMA初始化
// @param		dma_ch				DAM通道
// @param		src_addr			源地址
// @param		des_addr			目标地址
// @param		size				数据长度
// @return		void
// Sample usage:					camera_dma_init(DMA1_Channel4, GPIOA->ODR, GPIOC->ODR, 8);
//-------------------------------------------------------------------------------------------------------------------
void camera_dma_init ( uint32_t src_addr, uint32_t des_addr, uint32_t size)
{

    dma_single_data_parameter_struct dma_init_struct;
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1, DMA_CH7);

    dma_single_data_para_struct_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr = (uint32_t)des_addr;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = size;
    dma_init_struct.periph_addr = src_addr;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI7);
		
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(DMA1_Channel7_IRQn, 0,0);
    dma_interrupt_enable(DMA1,DMA_CH7,DMA_CHXCTL_FTFIE);	
    dma_interrupt_flag_clear(DMA1,DMA_CH7,DMA_INT_FLAG_FTF);
	
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		摄像头GPIO触发定时器初始化
// @param		timern				定时器通道
// @param		pin					引脚号
// @return		void
// Sample usage:					camera_tim_etr_init(TIM_3_ETR_D06);  						// 定时器ERT触发是固定的引脚禁止随意修改
//-------------------------------------------------------------------------------------------------------------------
void camera_tim_etr_init (void)
{
    timer_parameter_struct timer_initpara;
//    timer_ic_parameter_struct timer_icinitpara;
	
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_TIMER7);
		
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_0);
    gpio_af_set(GPIOA, GPIO_AF_3, GPIO_PIN_0);      

    timer_deinit(TIMER7);

    /* TIMER2 configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 65535;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER7,&timer_initpara);
		
    timer_external_clock_mode1_config(TIMER7,TIMER_EXT_TRI_PSC_OFF,TIMER_ETP_FALLING,0);

    timer_input_trigger_source_select(TIMER7,TIMER_SMCFG_TRGSEL_ETIFP);
    timer_master_slave_mode_config(TIMER7,TIMER_MASTER_SLAVE_MODE_ENABLE);
    timer_slave_mode_select(TIMER7,TIMER_SLAVE_MODE_EVENT);
		
		//timer_dma_transfer_config(TIMER1,TIMER_DMACFG_DMATA_CH0CV,TIMER_DMACFG_DMATC_1TRANSFER);
    timer_enable(TIMER7);
    timer_dma_enable(TIMER7,TIMER_DMA_TRGD);
		
}



void dcmi_dma_init(uint32_t DMA_Memory0BaseAddr,uint16_t DMA_BufferSize)
{ 
    dma_single_data_parameter_struct dma_single_struct;
	 /* DCI DMA configuration */ 
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1,DMA_CH7);
    dma_single_struct.periph_addr= (uint32_t)DCI_DR_ADDRESS;
    dma_single_struct.memory0_addr= (uint32_t)(DMA_Memory0BaseAddr);
    dma_single_struct.direction= DMA_PERIPH_TO_MEMORY;
    dma_single_struct.number= DMA_BufferSize;
    dma_single_struct.periph_inc= DMA_PERIPH_INCREASE_DISABLE;
    dma_single_struct.memory_inc= DMA_MEMORY_INCREASE_ENABLE;
    dma_single_struct.periph_memory_width= DMA_PERIPH_WIDTH_32BIT;
    dma_single_struct.priority= DMA_PRIORITY_HIGH;
    dma_single_struct.circular_mode= DMA_CIRCULAR_MODE_ENABLE;
    dma_single_data_mode_init(DMA1,DMA_CH7,&dma_single_struct);

    dma_channel_subperipheral_select(DMA1,DMA_CH7,DMA_SUBPERI1);
		
    dma_channel_enable(DMA1, DMA_CH7);//开启DMA2,Stream1 
    dci_capture_enable();//DCMI捕获使能
} 


void dcmi_init(void)
{
	 dci_parameter_struct dci_struct;
		   /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);

	/* configure the USART0 Tx pin and USART0 Rx pin */
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_4|GPIO_PIN_6);
    gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
	  gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_DCI);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_4|GPIO_PIN_6);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11);
    
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ,GPIO_PIN_4|GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ,GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ,GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11);
    
    dci_deinit();
		
    dci_struct.capture_mode=DCI_CAPTURE_MODE_CONTINUOUS;
    dci_struct.clock_polarity=  DCI_CK_POLARITY_RISING;
    dci_struct.hsync_polarity= DCI_HSYNC_POLARITY_LOW;
    dci_struct.vsync_polarity= DCI_VSYNC_POLARITY_LOW;
    dci_struct.frame_rate= DCI_FRAME_RATE_ALL;
    dci_struct.interface_format= DCI_INTERFACE_FORMAT_8BITS;
    dci_init(&dci_struct);
		
    dci_interrupt_enable(DCI_INT_EF);
    dci_enable();
		
    /* configure USART0 Tx as alternate function push-pull */
    
    nvic_irq_enable(DCI_IRQn, 1, 2);
	
 
}


