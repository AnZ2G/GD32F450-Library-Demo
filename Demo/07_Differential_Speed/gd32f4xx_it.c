/*!
    \file    gd32f4xx_it.c
    \brief   interrupt service routines
    
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

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
#include "gd32f4xx_it.h"
#include "gd32f450_board.h"
#include "zf_camera.h"
#include "motor.h"
#include "Encoder.h"

#include "SEEKFREE_MT9V03X.h"
uint16_t readvalue1 = 0, readvalue2 = 0;
__IO uint16_t ccnumber = 0;
__IO uint32_t count = 0;
__IO uint16_t fre = 0;
extern uint8_t mt9v03x_finish_flag;	
extern  uint8_t	receive[3];
extern  uint8_t	receive_num;
extern  uint8_t	uart_receive_flag;
extern  uint16_t flame_cnt;
extern  uint16_t ms_cnt;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
	 delay_decrement();
}
void USART0_IRQHandler(void)
{
    if((RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) && 
       (RESET != usart_flag_get(USART0, USART_FLAG_RBNE))){
				usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
				 usart_flag_clear(USART0, USART_FLAG_RBNE);
				receive[receive_num] = (uint8_t)usart_data_receive(USART0);
				receive_num++;

				if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
				if(3 == receive_num)
				{
					receive_num = 0;
					uart_receive_flag = 1;
				}
    }
}

void DMA1_Channel7_IRQHandler(void)
{   
	if( RESET !=dma_interrupt_flag_get(DMA1,DMA_CH7,DMA_CHXCTL_FTFIE)){
	  mt9v03x_finish_flag = 1;
		led_toggle(LED3);
		dma_interrupt_flag_clear(DMA1,DMA_CH7,DMA_INT_FLAG_FTF);
	  dma_interrupt_disable(DMA1,DMA_CH7,DMA_CHXCTL_FTFIE);
		dma_channel_disable(DMA1,DMA_CH7);
	}
	if( RESET !=dma_interrupt_flag_get(DMA1,DMA_CH7,DMA_INT_FLAG_FEE))led_toggle(LED3);
	if( RESET !=dma_interrupt_flag_get(DMA1,DMA_CH7,DMA_INT_FLAG_SDE))led_toggle(LED3);
	if( RESET !=dma_interrupt_flag_get(DMA1,DMA_CH7,DMA_INT_FLAG_TAE))led_toggle(LED3);
	if( RESET !=dma_interrupt_flag_get(DMA1,DMA_CH7,DMA_INT_FLAG_HTF))led_toggle(LED3);
//	dma_interrupt_disable(DMA1,DMA_CH7,DMA_CHXCTL_FTFIE);
//		dma_channel_disable(DMA1,DMA_CH7);
}
void EXTI5_9_IRQHandler(void)
{
	if(SET == exti_interrupt_flag_get(EXTI_9)){
		exti_flag_clear(EXTI_9);
		led_toggle(LED1);
		dma_transfer_number_config(DMA1,DMA_CH7,MT9V03X_H*MT9V03X_W);
	  dma_channel_enable(DMA1, DMA_CH7);

		//dma_channel_enable(DMA1, DMA_CH0);
	}
	if(SET == exti_interrupt_flag_get(EXTI_8)){
		exti_flag_clear(EXTI_8);
		
	}
  if(SET == exti_interrupt_flag_get(EXTI_7)){
		exti_flag_clear(EXTI_7);
		
	}
	if(SET == exti_interrupt_flag_get(EXTI_6)){
		exti_flag_clear(EXTI_6);
		
	}
	if(SET == exti_interrupt_flag_get(EXTI_5)){
		exti_flag_clear(EXTI_5);
		
	}
}

void TIMER1_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER1,TIMER_INT_CH0)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER1,TIMER_INT_CH0);
led_toggle(LED2);
        if(0 == ccnumber){
            /* read channel 0 capture value */
            readvalue1 = timer_channel_capture_value_register_read(TIMER1,TIMER_CH_0)+1;
            ccnumber = 1;
        }else if(1 == ccnumber){
            /* read channel 0 capture value */
            readvalue2 = timer_channel_capture_value_register_read(TIMER1,TIMER_CH_0)+1;

            if(readvalue2 > readvalue1){
                count = (readvalue2 - readvalue1); 
            }else{
                count = ((0xFFFF - readvalue1) + readvalue2); 
            }

            fre = 1000000 / count;
            printf("fre is %d\n",fre);
            ccnumber = 0;
        }
    }
}

void TIMER6_IRQHandler(void)
{
    if(timer_interrupt_flag_get(TIMER6,TIMER_INT_FLAG_UP) != RESET)
    {
        timer_interrupt_flag_clear(TIMER6,TIMER_INT_FLAG_UP);
        flame_cnt++;
    }
}

void TIMER0_UP_TIMER9_IRQHandler(void)
{
    if(timer_interrupt_flag_get(TIMER9,TIMER_INT_FLAG_UP) != RESET)
    {
        timer_interrupt_flag_clear(TIMER9,TIMER_INT_FLAG_UP);
        motor_l.real_speed = Encoder_Read(Encoder2);
        motor_r.real_speed = Encoder_Read(Encoder1);
        motor_error();
        motor_control();
    }
}

void TIMER0_TRG_CMT_TIMER10_IRQHandler(void)
{
    if(timer_interrupt_flag_get(TIMER10,TIMER_INT_FLAG_UP) != RESET)
    {
        timer_interrupt_flag_clear(TIMER10,TIMER_INT_FLAG_UP);

    }
}

void DCI_IRQHandler(void)
{
	   if(dci_interrupt_flag_get(DCI_INT_FLAG_EF)==SET)//Ö¡ÖÐ¶Ï
    {
        dci_interrupt_flag_clear(DCI_INT_FLAG_EF);//Çå³ýÖ¡ÖÐ¶Ï
//			dci_capture_disable();
        mt9v03x_finish_flag = 1;
    }
}
