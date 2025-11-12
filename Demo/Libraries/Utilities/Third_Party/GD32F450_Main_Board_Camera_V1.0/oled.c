/********************************************************************************************************************
* @file				oled.c
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-8-17
********************************************************************************************************************/

#include "oled.h"
#include "oledfont.h"
#include "stdlib.h"
#include "string.h" 	
#include "systick.h"
#include "gd32f4xx.h"
#include "wh_i2c.h"
#include "wh_gpio.h"
#include "wh_uart.h"
#include "Track.h"

uint8_t IMG_zip[RowMax][ColumnMax];

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED写字节
// @param		dat					写入的字节
// @param		cmd					写入命令或者数据
// 	@arg		OLED_CMD: 			写入命令
// 	@arg		OLED_DATA: 			写入数据
// @return		void
// Sample usage:			OLED_WR_Byte(0x00, OLED_CMD);//OLED写入命令0x00
//-------------------------------------------------------------------------------------------------------------------
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
    i2c_send_reg_byte(0x78, cmd, dat);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED初始化
// @return		void
// Sample usage:			OLED_Init();//OLED初始化
//-------------------------------------------------------------------------------------------------------------------			    
void OLED_Init(void)
{

/**************初始化SSD1306*****************/	
	OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/
	OLED_WR_Byte(0x00,OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x40,OLED_CMD); /*set display start line*/ 
	OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_WR_Byte(0xFF,OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x3F,OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0XF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x30,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD); /*set charge pump disable*/ 
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
}  

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED设置起始页
// @param		x					起始x坐标（点）
// @param		y					起始y坐标（页）
// @return		void
// Sample usage:			OLED_Set_Pos(0,0);//设置(0,0)（左上角）为起始坐标
//-------------------------------------------------------------------------------------------------------------------
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
 	OLED_WR_Byte(YLevel+y,OLED_CMD);
	OLED_WR_Byte((((x)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x)&0x0f),OLED_CMD); 
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED清除页
// @param		page				清除的页数：0、2、4、6（某一行）、8（全屏）
// @return		void
// Sample usage:			Clr_Screen(0);//清除第0-1页（第一行）
//-------------------------------------------------------------------------------------------------------------------
void Clr_Screen(unsigned char page)
{
    unsigned char i,j;
    if(page < 8)                            //只清某一行（两页）
    {
        OLED_Set_Pos(0,page);
        for(j = 0; j < 128; j++)
				{
            i2c_send_reg_byte(0x78, 0x40, 0x00);
				}//end of for(j = 0; j < 128; j++)
				OLED_Set_Pos(0,page+1);
        for(j = 0; j < 128; j++)
        {
            i2c_send_reg_byte(0x78, 0x40, 0x00);
        }//end of for(j = 0; j < 128; j++)   
    }//end of if(page < 8)   
    else if(page == 8)              //清全屏
    {
			  page = 0;
        for(i = 0; i < 8; i++)
        {
						OLED_Set_Pos(0,page);
						for(j = 0; j < 128; j++)
						{
								i2c_send_reg_byte(0x78, 0x40, 0x00);
						}//end of for(j = 0; j < 128; j++)
						page++;
    	  }//end of for(i = 0; i < 8; i++)
    }//end of else if(page == 8) 
 	  
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示字符
// @param		x				起始x坐标（点）
// @param		y				起始y坐标（页）
// @param		chr				显示的字符
// @param		Char_Size		字符尺寸（16、12）
// @return		void
// Sample usage:			OLED_ShowChar(0, 0, 'C', 16);//在(0,0)显示字符'C'（尺寸为16）
//-------------------------------------------------------------------------------------------------------------------
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0, *data_temp;	
	c=chr-' ';//得到偏移后的值		
	data_temp = acsii_buffer[c];
	if(x>127){x=0;y=y+2;}
	if(Char_Size ==16)
	{
	    OLED_Set_Pos(x,y);
		for (i = 0; i < 8; i++)
		{					
			i2c_send_reg_byte(0x78, 0x40, *data_temp);
			data_temp++;
		}//end of for (i = 0; i < 8; i++)
	    OLED_Set_Pos(x,y + 1);
		for (i = 0; i < 8; i++)
		{					
			i2c_send_reg_byte(0x78, 0x40, *data_temp);
			data_temp++;
		}//end of for (i = 0; i < 8; i++)
	}
	else
	{	
//				OLED_Set_Pos(x,y);
//				for(i=0;i<6;i++)
//				OLED_WR_Byte(F6x8_oled[c][i],OLED_DATA);
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		乘方运算函数
// @param		m				被乘数
// @param		n				次方
// @return		result			结果
// Sample usage:			uint32_t x;x = oled_pow(3,4);//x = 3^4
//-------------------------------------------------------------------------------------------------------------------
uint32_t oled_pow(uint8_t m,uint8_t n)
{
		uint32_t result=1;	 
		while(n--)result*=m;    
		return result;
}	


//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示数字
// @param		x				起始x坐标（点）
// @param		y				起始y坐标（页）
// @param		num				显示的数字
// @param		len				数字长度
// @param		size2			字符尺寸（16、12）
// @return		void			
// Sample usage:			OLED_ShowNum(0, 0, 114514, 6, 16);//在(0,0)显示数字"114514",字符大小为16
//-------------------------------------------------------------------------------------------------------------------
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
		uint8_t t,temp;
		uint8_t enshow=0;						   
		for(t=0;t<len;t++)
		{
				temp=(num/oled_pow(10,len-t-1))%10;
				if(enshow==0&&t<(len-1))
				{
						if(temp==0)
						{
							OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
							continue;
						}
						else enshow=1; 				 
			  }
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
		}
} 

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示字符串
// @param		x				起始x坐标（点）
// @param		y				起始y坐标（页）
// @param		*chr			显示的字符串地址
// @param		Char_Size		字符尺寸（16、12）
// @return		void
// Sample usage:			OLED_ShowString(0, 0, "yes", 16);//在(0,0)显示字符串"yes"，字符大小为16
//-------------------------------------------------------------------------------------------------------------------
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t Char_Size)
{
		unsigned char j=0;
		while (chr[j]!='\0')
		{		
				OLED_ShowChar(x,y,chr[j],Char_Size);
				x+=8;
				if(x>120)
		{
				x=0;
				y+=2;
		}
				j++;
		}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示字符（反白显示）
// @param		y				起始y坐标（页）
// @param		x				起始x坐标（点）
// @param		chr				显示的字符
// @param		Char_Size		字符尺寸（16、12）
// @return		void
// Sample usage:			OLED_ShowChar_1(2, 0, 'C', 16);//在(0,2)显示字符'C'（反白显示，尺寸为16）
//-------------------------------------------------------------------------------------------------------------------
void OLED_ShowChar_1(uint8_t y,uint8_t x,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0, *data_temp;	
	c=chr-' ';//得到偏移后的值		
	data_temp = acsii_buffer[c];
	if(x>127){x=0;y=y+2;}
	if(Char_Size ==16)
	{
	    OLED_Set_Pos(x,y);
		for (i = 0; i < 8; i++)
		{					
			i2c_send_reg_byte(0x78, 0x40, ~*data_temp);
			data_temp++;
		}//end of for (i = 0; i < 8; i++)
	    OLED_Set_Pos(x,y + 1);
		for (i = 0; i < 8; i++)
		{					
			i2c_send_reg_byte(0x78, 0x40, ~*data_temp);
			data_temp++;
		}//end of for (i = 0; i < 8; i++)
	}
	else
	{	
//				OLED_Set_Pos(x,y);
//				for(i=0;i<6;i++)
//				OLED_WR_Byte(F6x8_oled[c][i],OLED_DATA);
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示8x16字符串（选择反白或正常显示）
// @param		y				起始y坐标（页）
// @param		x				起始x坐标（点）
// @param		Char_Size		是否反白显示（1为反白显示，0为正常显示）
// @param		*chr			显示的字符串地址
// @return		void
// Sample usage:			print_str8x16(0, 0, 1, "yes");//在(0,0)显示字符串"yes"，反白显示
//-------------------------------------------------------------------------------------------------------------------
void print_str8x16(uint8_t y,uint8_t x,uint8_t Char_Size,uint8_t *chr)
{
		unsigned char j=0;
		if(Char_Size==1)
		{
			while (chr[j]!='\0')
		{		
		    OLED_ShowChar_1(y,x,chr[j],16);
				x+=8;
				if(x>120)
        {
				    x=0;
				    y+=2;
				}
				j++;
		}
		}
		else
		{
			while (chr[j]!='\0')
		{		
		    OLED_ShowChar(x,y,chr[j],16);
				x+=8;
				if(x>120)
        {
				    x=0;
				    y+=2;
				}
				j++;
		}
		}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示8x16字符（选择反白或正常显示）
// @param		y				起始y坐标（页）
// @param		x				起始x坐标（点）
// @param		Char_Size		是否反白显示（1为反白显示，0为正常显示）
// @param		*chr			显示的字符地址
// @return		void
// Sample usage:			uint8_t chr = 'C';print_chr8x16(0, 0, 1, &chr);//在(0,0)显示字符chr，反白显示
//-------------------------------------------------------------------------------------------------------------------
void print_char8x16(uint8_t y,uint8_t x,uint8_t Char_Size,uint8_t *chr)
{
		if(Char_Size==1)
		{
			OLED_ShowChar_1(y,x,*chr,16);
		}
		else
		{
			OLED_ShowChar(x,y,*chr,16);
		}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED清除页的部分列
// @param		y				起始y坐标（页）
// @param		start			起始x坐标（点）
// @param		end				终止x坐标（点）
// @return		void
// Sample usage:			clr_Screen(0,0,127);//清除第0、1页（第一行）的第0个点至第127个点
//-------------------------------------------------------------------------------------------------------------------
void clr_Screen(unsigned char page,unsigned char start,unsigned char end)
{
    unsigned char clr_sum;
    unsigned char j;
    clr_sum = end - start;//所要清除的点的个数
    
    OLED_Set_Pos(start, page);
    for(j = 0; j < clr_sum; j++)
    {
        OLED_WR_Byte(0,OLED_DATA);
    }//end of for(j = 0; j < 128; j++)
    OLED_Set_Pos(start, page+1);
    for(j = 0; j < clr_sum; j++)
    {
       OLED_WR_Byte(0,OLED_DATA);
    }//end of for(j = 0; j < 128; j++)    
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示摄像头图像
// @param		value			二值化阈值
// @return		void
// Sample usage:			OLED_camera(120);//清除第0、1页（第一行）的第0个点至第127个点
//-------------------------------------------------------------------------------------------------------------------
void OLED_camera(uint8_t value)
{ 	 
    uint8_t i = 0, j = 0,temp=0,page=0;
    OLED_Set_Pos(0, page);
    for(i=0;i<56;i+=8)// 56行 
    {
        for(j=0;j<188;j+=2) 
        { 
            temp=0;
            if(Img[(0+i)*2][j]>=value) temp|=1;
            if(Img[(1+i)*2][j]>=value) temp|=2;
            if(Img[(2+i)*2][j]>=value) temp|=4;
            if(Img[(3+i)*2][j]>=value) temp|=8;
            if(Img[(4+i)*2][j]>=value) temp|=0x10;
            if(Img[(5+i)*2][j]>=value) temp|=0x20;
            if(Img[(6+i)*2][j]>=value) temp|=0x40;
            if(Img[(7+i)*2][j]>=value) temp|=0x80;
            i2c_send_reg_byte(0x78, 0x40, temp);  	  	  	  
        }
        page++;
        OLED_Set_Pos(0, page);
    }  
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示摄像头图像
// @param		value			二值化阈值
// @return		void
// Sample usage:			OLED_camera(120);//清除第0、1页（第一行）的第0个点至第127个点
//-------------------------------------------------------------------------------------------------------------------
void OLED_camera_zip(uint8_t value)
{ 	 
    uint8_t i = 0, j = 0,temp=0,page=0;
    page=0;
    OLED_Set_Pos(0, page);
    for(i=0;i<56;i+=8)// 56行 
    {
        for(j=0;j<94;j+=1) 
        { 
            temp=0;
            if(IMG_zip[(0+i)][j]>=value) temp|=1;
            if(IMG_zip[(1+i)][j]>=value) temp|=2;
            if(IMG_zip[(2+i)][j]>=value) temp|=4;
            if(IMG_zip[(3+i)][j]>=value) temp|=8;
            if(IMG_zip[(4+i)][j]>=value) temp|=0x10;
            if(IMG_zip[(5+i)][j]>=value) temp|=0x20;
            if(IMG_zip[(6+i)][j]>=value) temp|=0x40;
            if(IMG_zip[(7+i)][j]>=value) temp|=0x80;
            i2c_send_reg_byte(0x78, 0x40, temp);  	  	  	  
        }
        page++;
        OLED_Set_Pos(0, page);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED显示图像
// @param		buffer			显示的数组
// @param		value			二值化阈值
// @return		void
// Sample usage:			OLED_camera(120);//清除第0、1页（第一行）的第0个点至第127个点
//-------------------------------------------------------------------------------------------------------------------
void OLED_camera_any(uint8_t buffer[60][94], uint8_t value)
{ 	 
    uint8_t i = 0, j = 0,temp=0,page=0;
    page=0;
    OLED_Set_Pos(0, page);
    for(i=0;i<56;i+=8)// 56行 
    {
        for(j=0;j<94;j+=1) 
        { 
            temp=0;
            if(buffer[(0+i)][j]>=value) temp|=1;
            if(buffer[(1+i)][j]>=value) temp|=2;
            if(buffer[(2+i)][j]>=value) temp|=4;
            if(buffer[(3+i)][j]>=value) temp|=8;
            if(buffer[(4+i)][j]>=value) temp|=0x10;
            if(buffer[(5+i)][j]>=value) temp|=0x20;
            if(buffer[(6+i)][j]>=value) temp|=0x40;
            if(buffer[(7+i)][j]>=value) temp|=0x80;
            i2c_send_reg_byte(0x78, 0x40, temp);  	  	  	  
        }
        page++;
        OLED_Set_Pos(0, page);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED开机动画内部函数
// @return		void
// Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_ShowSchool(uint8_t y, uint8_t pos)
{      	
    uint8_t i,j,k;
    for(k=0;k<8-y-pos;k++)
    {
        OLED_Set_Pos(0,k+y);
        for (i=0;i<8;i++)
        {			
            for(j=0;j<16;j++)
            {		
                i2c_send_reg_byte(0x78, 0x40, bmp_school_buffer[i+k*8+pos*8][j]);
            }
        }//end of for (i = 0; i < 8; i++)
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED开机动画内部函数
// @return		void
// Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_ShowSchool_1(uint8_t y, uint8_t pos)
{      	
    uint8_t i,j,k;
    for(k=0;k<3-pos;k++)
    {
        OLED_Set_Pos(0,k+y);
        for (i=0;i<8;i++)
        {			
            for(j=0;j<16;j++)
            {		
                i2c_send_reg_byte(0x78, 0x40, bmp_school_buffer_1[i+k*8+pos*8][j]);
            }
        }//end of for (i = 0; i < 8; i++)
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		OLED开机动画
// @return		void
// Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_OpeningAnimation(void)
{
    /* 显示校徽 */
    Clr_Screen(8);
    OLED_ShowSchool(0,7);
    delay_1ms(1);
    OLED_ShowSchool(0,6);
    delay_1ms(1);
    OLED_ShowSchool(0,5);
    delay_1ms(1);
    OLED_ShowSchool(0,4);
    delay_1ms(1);
    OLED_ShowSchool(0,3);
    delay_1ms(1);
    OLED_ShowSchool(0,2);
    delay_1ms(1);
    OLED_ShowSchool(0,1);
    delay_1ms(1);
    OLED_ShowSchool(0,0);
    delay_1ms(300);
    Clr_Screen(0);
    OLED_ShowSchool(1,0);
    delay_1ms(1);
    Clr_Screen(1);
    OLED_ShowSchool(2,0);
    delay_1ms(1);
    Clr_Screen(2);
    OLED_ShowSchool(3,0);
    delay_1ms(1);
    Clr_Screen(3);
    OLED_ShowSchool(4,0);
    delay_1ms(1);
    Clr_Screen(4);
    OLED_ShowSchool(5,0);
    delay_1ms(1);
    Clr_Screen(5);
    OLED_ShowSchool(6,0);
    delay_1ms(1);
    Clr_Screen(6);
    OLED_ShowSchool(7,0);

    /* 显示校徽 */
    Clr_Screen(8);
    OLED_ShowSchool_1(0,2);
    delay_1ms(1);
    OLED_ShowSchool_1(0,1);
    delay_1ms(1);
    OLED_ShowSchool_1(0,0);
    delay_1ms(1);
    Clr_Screen(0);
    OLED_ShowSchool_1(1,0);
    delay_1ms(1);
    Clr_Screen(1);
    OLED_ShowSchool_1(2,0);
    delay_1ms(300);
    Clr_Screen(2);
    OLED_ShowSchool_1(3,0);
    delay_1ms(1);
    Clr_Screen(3);
    OLED_ShowSchool_1(4,0);
    delay_1ms(1);
    Clr_Screen(4);
    OLED_ShowSchool_1(5,0);
    delay_1ms(1);
    Clr_Screen(5);
    OLED_ShowSchool_1(6,0);
    delay_1ms(1);
    Clr_Screen(6);
}
