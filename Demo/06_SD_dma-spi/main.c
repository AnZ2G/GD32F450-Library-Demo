#include "gd32f4xx.h"
#include "headfile.h"
#include "systick.h"
#include <stdio.h>
#include "tf_spi.h"

uint8_t buffer[512] = {"asdfg"}, buffr[1527] = {"大力开发阶段撒反对飓风阿瑟房间里收到v安瑟伦覅居住了据阿斯俄方；拉瑟就出色的从v骄傲而从serfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftdrethxasefdfasgfrsrgsdrgfaserfaeffffasrgdrgfzsdrgsrtgbtgfmjuyfg8itkydjhtfgjntf67w4hy34hyrikbdrfvjgftd"};
uint8_t buf[512],bufr[1024];

int main ()
{
    systick_config();

    /* 配置PD5为TXD引脚 */
    uart_tx_init(GPIOD, GPIO_PIN_5, RCU_GPIOD);

    /* 配置PD6为RXD引脚 */
    uart_rx_init(GPIOD, GPIO_PIN_6, RCU_GPIOD);

    /* 配置UART，使用USART1，波特率设置为115200 */
    uart_init(RCU_USART1, USART1, 115200U);    
    
    printf("a usart transmit test example!");
	
	  DMA_init((uint32_t*)buffr, sector_size); 
	  printf("init:%d\r\n",SD_Init_Config());
	  printf("write:%d\r\n",SD_SendBlock(buffer, 0x00));
	  SD_RecvData(buf, 0x00, 5);
	  printf("buf:%s\r\n",buf);
	  printf("write:%d\r\n",SD_WriteDisk2(buffr,0x01,1200));
		SD_ReadDisk2(bufr, 0x01, 1200);
	  printf("bufr:%s\r\n",bufr);
	  erasure(2,3);
	  SD_ReadDisk2(er bufr, 0x01, 1200);
	  printf("bufr:%s\r\n",bufr);

	  while(1);
}

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}
