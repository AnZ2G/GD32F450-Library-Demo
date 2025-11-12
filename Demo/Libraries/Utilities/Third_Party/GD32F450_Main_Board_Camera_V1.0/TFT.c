/*********************************************************************************************************************
* @file				TFT.c
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			Main_Board_Camera_V1.0
* @date				2021-8-28
********************************************************************************************************************/

#include "TFT.h"
#include "headfile.h"
#include "systick.h"
#include "SEEKFREE_MT9V03X.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		TFT屏SPI初始化
// @return		void
// Sample usage:			SPI_Init();//初始化SPI
//-------------------------------------------------------------------------------------------------------------------
void TFT_SPI_Init(void)
{
    /* 初始化RES引脚 */
    rcu_periph_clock_enable(TFT_RES_GPIO_CLK); 
    gpio_output_init(TFT_RES_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TFT_RES_GPIO_PIN);
    gpio_set_output(TFT_RES_GPIO_PORT, TFT_RES_GPIO_PIN, 0);

    /* 初始化DC引脚 */
    rcu_periph_clock_enable(TFT_DC_GPIO_CLK); 
    gpio_output_init(TFT_DC_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TFT_DC_GPIO_PIN);
    gpio_set_output(TFT_DC_GPIO_PORT, TFT_DC_GPIO_PIN, 0);

//    /* 初始化MISO引脚 */
//    rcu_periph_clock_enable(TFT_MISO_GPIO_CLK);
//    gpio_output_init(TFT_MISO_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TFT_MISO_GPIO_PIN);
//    gpio_af_set(TFT_MISO_GPIO_PORT, GPIO_AF_5, TFT_MISO_GPIO_PIN);
//    gpio_set_output(TFT_MISO_GPIO_PORT, TFT_MISO_GPIO_PIN, 0);
//    gpio_mode_set(TFT_MISO_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, TFT_MISO_GPIO_PIN);

    /* 初始化SCK引脚 */
    rcu_periph_clock_enable(TFT_SCK_GPIO_CLK);
    gpio_output_init(TFT_SCK_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TFT_SCK_GPIO_PIN);
    gpio_af_set(TFT_SCK_GPIO_PORT, GPIO_AF_5, TFT_SCK_GPIO_PIN);
    gpio_set_output(TFT_SCK_GPIO_PORT, TFT_SCK_GPIO_PIN, 0);
    gpio_mode_set(TFT_SCK_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, TFT_SCK_GPIO_PIN);

    rcu_periph_clock_enable(TFT_MISO_GPIO_CLK); 
    gpio_output_init(TFT_MISO_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TFT_MISO_GPIO_PIN);
    gpio_set_output(TFT_MISO_GPIO_PORT, TFT_MISO_GPIO_PIN, 0);
    rcu_periph_clock_enable(TFT_SCK_GPIO_CLK); 
    gpio_output_init(TFT_SCK_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TFT_SCK_GPIO_PIN);
    gpio_set_output(TFT_SCK_GPIO_PORT, TFT_SCK_GPIO_PIN, 0);

    /* 初始化CS引脚 */
    rcu_periph_clock_enable(TFT_CS_GPIO_CLK); 
    gpio_output_init(TFT_CS_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TFT_CS_GPIO_PIN);
    gpio_set_output(TFT_CS_GPIO_PORT, TFT_CS_GPIO_PIN, 0);

    /* 初始化SPI外设 */
    spi_config(SPI_PERIPH, SPI_CLK);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TFT屏写命令
// @param		cmd			写入的命令				
// @return		void
// Sample usage:			TFTSPI_Write_Cmd(0x11);//向TFT屏写入命令0x11
//-------------------------------------------------------------------------------------------------------------------
void TFTSPI_Write_Cmd(uint8_t cmd)
{

////    /* 设置SPI数据格式为8位 */
////    spi_i2s_data_frame_format_config(SPI_PERIPH, SPI_FRAMESIZE_8BIT);
//    /* DC脚置0 */
//    gpio_set_output(TFT_DC_GPIO_PORT, TFT_DC_GPIO_PIN, 0);

//    /* SPI写入数据 */
//	spi_i2s_data_transmit(SPI_PERIPH,cmd);
//	while(spi_i2s_flag_get(SPI_PERIPH,SPI_FLAG_TBE)==RESET);

    uint8_t i;
  
    gpio_set_output(TFT_DC_GPIO_PORT, TFT_DC_GPIO_PIN, 0);
    for(i=0;i<8;i++)
    {
        gpio_set_output(TFT_SCK_GPIO_PORT, TFT_SCK_GPIO_PIN, 0);
        if(cmd&0x80){gpio_set_output(TFT_MISO_GPIO_PORT, TFT_MISO_GPIO_PIN, 1);}// SDI=1
        else{gpio_set_output(TFT_MISO_GPIO_PORT, TFT_MISO_GPIO_PIN, 0);}// SDI=0 
        gpio_set_output(TFT_SCK_GPIO_PORT, TFT_SCK_GPIO_PIN, 1);
        cmd = (cmd<<1);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TFT屏写数据
// @param		cmd			写入的数据				
// @return		void
// Sample usage:			TFTSPI_Write_Byte(0x55);//向TFT屏写入数据0x55
//-------------------------------------------------------------------------------------------------------------------
void TFTSPI_Write_Byte(uint8_t byte)
{
////    /* 设置SPI数据格式为8位 */
////    spi_i2s_data_frame_format_config(SPI_PERIPH, SPI_FRAMESIZE_8BIT);

//    /* DC脚置1 */
//    gpio_set_output(TFT_DC_GPIO_PORT, TFT_DC_GPIO_PIN, 1);

//    /* SPI写入数据 */
//	spi_i2s_data_transmit(SPI_PERIPH,byte);
//	while(spi_i2s_flag_get(SPI_PERIPH,SPI_FLAG_TBE)==RESET);

    uint8_t i;
  
    gpio_set_output(TFT_DC_GPIO_PORT, TFT_DC_GPIO_PIN, 1);
    for(i=0;i<8;i++)
    {
        gpio_set_output(TFT_SCK_GPIO_PORT, TFT_SCK_GPIO_PIN, 0);
        if(byte&0x80){gpio_set_output(TFT_MISO_GPIO_PORT, TFT_MISO_GPIO_PIN, 1);}// SDI=1
        else{gpio_set_output(TFT_MISO_GPIO_PORT, TFT_MISO_GPIO_PIN, 0);}// SDI=0 
        gpio_set_output(TFT_SCK_GPIO_PORT, TFT_SCK_GPIO_PIN, 1);
        byte = (byte<<1);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TFT屏写词组（16位）
// @param		dat			写入的数据				
// @return		void
// Sample usage:			TFTSPI_Write_Byte(0x55);//向TFT屏写入数据0x55
//-------------------------------------------------------------------------------------------------------------------
void TFTSPI_Write_Word(uint16_t dat)
{  		 
//    /* 设置SPI数据格式为8位 */
//    spi_i2s_data_frame_format_config(SPI_PERIPH, SPI_FRAMESIZE_16BIT);

//      /* DC脚置1 */
//    gpio_set_output(TFT_DC_GPIO_PORT, TFT_DC_GPIO_PIN, 1);	

//	spi_i2s_data_transmit(SPI_PERIPH,dat);
//	while(spi_i2s_flag_get(SPI_PERIPH,SPI_FLAG_TBE)==RESET);

    uint8_t i;

    /* DC脚置1 */
    gpio_set_output(TFT_DC_GPIO_PORT, TFT_DC_GPIO_PIN, 1);	

    for(i=0;i<16;i++)
    {
        gpio_set_output(TFT_SCK_GPIO_PORT, TFT_SCK_GPIO_PIN, 0);	 
        if(dat&0x8000){gpio_set_output(TFT_MISO_GPIO_PORT, TFT_MISO_GPIO_PIN, 1);}// SDI=1
        else{gpio_set_output(TFT_MISO_GPIO_PORT, TFT_MISO_GPIO_PIN, 0);}// SDI=0
        gpio_set_output(TFT_SCK_GPIO_PORT, TFT_SCK_GPIO_PIN, 1);
        dat<<=1;    
    } 
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TFT屏初始化
// @param		type		显示方式：1:竖屏,2:横屏				
// @return		void
// Sample usage:			TFT_Init(2);//TFT屏初始化，显示方式为横屏显示
//-------------------------------------------------------------------------------------------------------------------
void TFT_Init(uint8_t type)
{ 	
  gpio_set_output(TFT_RES_GPIO_PORT, TFT_RES_GPIO_PIN, 0);
  delay_1ms(50);                   
  gpio_set_output(TFT_RES_GPIO_PORT, TFT_RES_GPIO_PIN, 1);
  delay_1ms(50);   
  TFTSPI_Write_Cmd(0x11);       		  	//关闭睡眠，振荡器工作
  delay_1ms(10);  
  TFTSPI_Write_Cmd(0x3a);       		  	//每次传送16位数据(VIPF3-0=0101)，每个像素16位(IFPF2-0=101)
  TFTSPI_Write_Byte(0x55);	
  TFTSPI_Write_Cmd(0x26);       		  	
  TFTSPI_Write_Byte(0x04);  
  TFTSPI_Write_Cmd(0xf2);              		//Driver Output Control(1)
  TFTSPI_Write_Byte(0x01);  
  TFTSPI_Write_Cmd(0xe0);              		//Driver Output Control(1)
  TFTSPI_Write_Byte(0x3f);
  TFTSPI_Write_Byte(0x25);
  TFTSPI_Write_Byte(0x1c);
  TFTSPI_Write_Byte(0x1e);
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x12);
  TFTSPI_Write_Byte(0x2a);
  TFTSPI_Write_Byte(0x90);
  TFTSPI_Write_Byte(0x24);
  TFTSPI_Write_Byte(0x11);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);  
  TFTSPI_Write_Cmd(0xe1);              //Driver Output Control(1)
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x20);
  TFTSPI_Write_Byte(0x05);
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x15);
  TFTSPI_Write_Byte(0xa7);
  TFTSPI_Write_Byte(0x3d);
  TFTSPI_Write_Byte(0x18);
  TFTSPI_Write_Byte(0x25);
  TFTSPI_Write_Byte(0x2a);
  TFTSPI_Write_Byte(0x2b);
  TFTSPI_Write_Byte(0x2b);  
  TFTSPI_Write_Byte(0x3a);    
  TFTSPI_Write_Cmd(0xb1);      //0xb1      	//设置屏幕刷新频率
  TFTSPI_Write_Byte(0x00);     //0x08		//DIVA=8
  TFTSPI_Write_Byte(0x00);	    //0x08		//VPA =8，约90Hz  
  TFTSPI_Write_Cmd(0xb4);              	//LCD Driveing control
  TFTSPI_Write_Byte(0x07);			//NLA=1,NLB=1,NLC=1  
  TFTSPI_Write_Cmd(0xc0);              //LCD Driveing control  Power_Control1 
  TFTSPI_Write_Byte(0x0a);    
  TFTSPI_Write_Byte(0x02);  
  TFTSPI_Write_Cmd(0xc1);              //LCD Driveing control
  TFTSPI_Write_Byte(0x02);  
  TFTSPI_Write_Cmd(0xc5);              //LCD Driveing control
  TFTSPI_Write_Byte(0x4f);
  TFTSPI_Write_Byte(0x5a);  
  TFTSPI_Write_Cmd(0xc7);              //LCD Driveing control
  TFTSPI_Write_Byte(0x40);  
  TFTSPI_Write_Cmd(0x2a);              //配置MCU可操作的LCD内部RAM横坐标起始、结束参数
  TFTSPI_Write_Byte(0x00);		//横坐标起始地址0x0000
  TFTSPI_Write_Byte(0x00);				
  TFTSPI_Write_Byte(0x00);		//横坐标结束地址0x007f(127)
  TFTSPI_Write_Byte(0xa8);                //7f  
  TFTSPI_Write_Cmd(0x2b);              //配置MCU可操作的LCD内部RAM纵坐标起始结束参数
  TFTSPI_Write_Byte(0x00);		//纵坐标起始地址0x0000
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);		//纵坐标结束地址0x009f(159)  
  TFTSPI_Write_Byte(0xb3);                //9f  
  TFTSPI_Write_Cmd(0x36);              //配置MPU和DDRAM对应关系
  if(type) 
  TFTSPI_Write_Byte(0xC0);                //竖屏显示          //MX=1,MY=1
  else
  TFTSPI_Write_Byte(0xA0);            	  //横屏显示
  
  TFTSPI_Write_Cmd(0xb7);              //LCD Driveing control
  TFTSPI_Write_Byte(0x00);		//CRL=0  
  TFTSPI_Write_Cmd(0x29);   		//开启屏幕显示
  TFTSPI_Write_Cmd(0x2c);   		//设置为LCD接收数据/命令模式  
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TFT屏设置起点终点
// @param		xs			起点横坐标
// @param		xe			终点横坐标
// @param		ys			起点纵坐标
// @param		ye			终点纵坐标				
// @return		void
// Sample usage:			TFTSPI_Set_Pos(2);//TFT屏初始化，显示方式为横屏显示
//-------------------------------------------------------------------------------------------------------------------
void TFTSPI_Set_Pos(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye)
{
  TFTSPI_Write_Cmd(0x2A);      	//Colulm addRSTs set
  TFTSPI_Write_Byte(0x00);		//行起始坐标高8位，始终为零
  TFTSPI_Write_Byte(xs);			//行起始坐标低8位
  TFTSPI_Write_Byte(0x00);		//行终止坐标高8位，始终为零
  TFTSPI_Write_Byte(xe);			//行终止坐标低8位  
  TFTSPI_Write_Cmd(0x2B);      	//Colulm addRSTs set
  TFTSPI_Write_Byte(0x00);		//列起始坐标高8位，始终为零
  TFTSPI_Write_Byte(ys);			//列起始坐标低8位
  TFTSPI_Write_Byte(0x00);		//列终止坐标高8位，始终为零
  TFTSPI_Write_Byte(ye);			//列终止坐标低8位  
  TFTSPI_Write_Cmd(0x2C);      	//GRAM接收MCU数据或命令
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TFT屏重置地址			
// @return		void
// Sample usage:			TFTSPI_Addr_Rst();//TFT屏重置地址
//-------------------------------------------------------------------------------------------------------------------
void TFTSPI_Addr_Rst(void)
{
  TFTSPI_Write_Cmd(0x2a);     //配置MCU可操作的LCD内部RAM横坐标起始、结束参数
  TFTSPI_Write_Byte(0x00);       //横坐标起始地址0x0000
  TFTSPI_Write_Byte(0x00);					
  TFTSPI_Write_Byte(0x00);       //横坐标结束地址0x007f(127)
  TFTSPI_Write_Byte(0xa8);       //7f  
  TFTSPI_Write_Cmd(0x2b);     //配置MCU可操作的LCD内部RAM纵坐标起始结束参数
  TFTSPI_Write_Byte(0x00);       //纵坐标起始地址0x0000
  TFTSPI_Write_Byte(0x00);
  TFTSPI_Write_Byte(0x00);       //纵坐标结束地址0x009f(159)
  TFTSPI_Write_Byte(0xb3);       //9f  
  TFTSPI_Write_Cmd(0x2C);     //GRAM接收MCU数据或命令
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TFT清屏	
// @param		color			清屏颜色。白色为0xffff		
// @return		void
// Sample usage:			TFTSPI_CLS(0xffff);//TFT显示全白
//-------------------------------------------------------------------------------------------------------------------
void TFTSPI_CLS(uint16_t color)
{
  unsigned int i,j;
  
  TFTSPI_Addr_Rst();
  
  for(i=0;i<TFT18W;i++)      //160
  {
    for(j=0;j<TFT18H;j++)
    {
      TFTSPI_Write_Word(color);
    }
  }
}

/**********************************************************
函数名称：TFTSPI_Draw_Dot()
入口参数：起始横坐标(0-127)，纵坐标(0-159),显示颜色uint16
出口参数：无
时间：2012-09-09
功能说明：画点
其他说明：0<=x<=127
0<=y<=159
**********************************************************/
void TFTSPI_Draw_Dot(uint8_t x,uint8_t y,uint16_t color_dat)
{
  TFTSPI_Set_Pos(x,y,x,y);
  TFTSPI_Write_Word(color_dat);
}
