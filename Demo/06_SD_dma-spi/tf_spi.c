//仅测试2.0版本SDHC卡

#include "tf_spi.h"
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include <math.h>

uint8_t SD_Type;//SD卡类型
uint8_t csd[16];//SD卡csd信息

	
//-------------------------------------------------------------------------------------------------------------------
// @brief		SPI初始化
// @param		void			
// @return	void
// Sample usage:			SPI_init(void)
//                     //初始化SPI3，打开DMA传输功能、NSS软件片选（分频值影响是否能操作SD卡）
//-------------------------------------------------------------------------------------------------------------------	
void SPI_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(rcu_spix);

    gpio_af_set(GPIOE, GPIO_AF_5, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
		
    spi_i2s_deinit(spix);
	
	  spi_parameter_struct spi_init_struct;

    spi_struct_para_init(&spi_init_struct);

    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_16;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
		
    spi_init(spix, &spi_init_struct);
		spi_dma_enable(spix, SPI_DMA_TRANSMIT);
		spi_nss_output_enable(spix);
		spi_enable(spix);
		spi_nss_internal_high(spix);
    SPI_ReadWrite_Byte(0xff);
		
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		DMA初始化
// @param		txbuffer     	传输存储地址		
// @param		num     	    DMA通道传输数据量	
// @return	void
// Sample usage:			    DMA_init((uint32_t*)buffr, 512);
//                           //初始化DMA1、CH1，buffr传输到外设SPI3，传输量512字节
//-------------------------------------------------------------------------------------------------------------------	
void DMA_init(uint32_t* txbuffer, uint32_t num)
{
    dma_single_data_parameter_struct dma_single_struct;

    rcu_periph_clock_enable(RCU_DMA1);
	
    dma_deinit(DMA1,channelx);
    dma_single_struct.periph_addr= (uint32_t)&SPI_DATA(spix);
    dma_single_struct.memory0_addr= (uint32_t)txbuffer;
    dma_single_struct.direction= DMA_MEMORY_TO_PERIPH;
    dma_single_struct.number= num;
    dma_single_struct.periph_inc= DMA_PERIPH_INCREASE_DISABLE;
    dma_single_struct.memory_inc= DMA_MEMORY_INCREASE_ENABLE;
    dma_single_struct.periph_memory_width= DMA_PERIPH_WIDTH_8BIT;
    dma_single_struct.priority= DMA_PRIORITY_HIGH;
    dma_single_struct.circular_mode= DMA_CIRCULAR_MODE_DISABLE;
    dma_single_data_mode_init(DMA1,channelx,&dma_single_struct);

    dma_channel_subperipheral_select(DMA1,channelx,DMA_SUBPERI4);
		dma_flag_clear(DMA1,channelx,DMA_FLAG_FTF);
    dma_channel_disable(DMA1, channelx);//传输时再打开，方便改变存储器地址

}

//-------------------------------------------------------------------------------------------------------------------
// @brief		向SD卡发送一个命令
// @param		cmd     	    命令号		
// @param		arg     	    命令参数	
// @param		crc     	    校验值（打开SPI功能后默认关闭校验功能）
// @return	uint8_t       返回SD卡响应值
// Sample usage:			    SD_SendCmd(CMD0,0,0x95);
//                           //发送CMD0命令格式，使SD卡进入IDLE状态
//-------------------------------------------------------------------------------------------------------------------	
uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1;	
    uint8_t Retry; 

    SPI_ReadWrite_Byte(cmd | 0x40);
    SPI_ReadWrite_Byte(arg >> 24);
    SPI_ReadWrite_Byte(arg >> 16);
    SPI_ReadWrite_Byte(arg >> 8);
    SPI_ReadWrite_Byte(arg);	  
    SPI_ReadWrite_Byte(crc); 
    if(cmd==CMD12)
		{
   			SPI_ReadWrite_Byte(0xff);
		}//Skip a stuff byte when stop reading
		
	  Retry=0X1F;
    do
    {
	      r1=SPI_ReadWrite_Byte(0xFF);
    }while((r1&0X80) && Retry--);	

    return r1;
}					  

//-------------------------------------------------------------------------------------------------------------------
// @brief		获取SD卡的CSD信息，包括容量和速度信息
// @param		csd_data     	    存放CSD的内存，至少16个字节		
// @return	uint8_t       0:NO_ERR；1:错误响应	
// Sample usage:			    SD_GetCSD(csd)
//                           //发送CMD9命令格式，csd存储16位SD卡信息
//-------------------------------------------------------------------------------------------------------------------											   
uint8_t SD_GetCSD(uint8_t *csd_data)
{
    uint8_t r1;	 
	
	  spi_nss_internal_low(spix);
    r1=SD_SendCmd(CMD9,0x00,0xff);
    if(r1==0)
    {
     r1=SD_RecvData(csd_data, 0x00,16); 
    }
    SD_DisSelect();

    if(r1)
		{
		    return 1;
		}
    else 
    {
		    return 0;
		}
} 

//-------------------------------------------------------------------------------------------------------------------
// @brief		获取SD卡总扇区数
// @param		csd     	    存放CSD的内存，至少16个字节	
// @return	uint8_t       0: 取出容量出错；其他:SD容量(扇区数/512字节)
// Sample usage:			    SD_GetSectorCount(csd)
//                           //csd存储16位SD卡信息后，读取SD卡容量信息
//-------------------------------------------------------------------------------------------------------------------												  
uint32_t SD_GetSectorCount(uint8_t* csd)
{
    uint32_t Capacity;  
    uint8_t n;
    uint32_t csize;  					    

    if(SD_GetCSD(csd)!=0)
		{
			  return 0;  
		}

    if((csd[0]&0xC0)==0x40)	 //V2.00卡
		{	
	      csize = (uint32_t)csd[8]&0x06 + ((uint32_t)csd[7] << 6) + ((uint32_t)csd[6] << 14) + 1;//正常678
	      Capacity = csize << 10;//得到扇区数		   
    }
	  else//V1.XX卡
    {	
	      n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
	      csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
	      Capacity= (uint16_t)csize << (n - 9);//得到扇区数 
    }
	 
   return Capacity;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		spi读写一个字节
// @param		TxData     	  spi发送值		
// @return	uint8_t       返回SD卡响应值
// Sample usage:			    SPI_ReadWrite_Byte(0xff);
//                           //发送0xff（可向SD卡提供8个脉冲）
//-------------------------------------------------------------------------------------------------------------------	
uint8_t SPI_ReadWrite_Byte(uint8_t TxData)
{    
    while (spi_i2s_flag_get(spix, SPI_FLAG_TBE) == RESET){;}//等待发送区空 
	  spi_i2s_data_transmit(spix, TxData); //通过外设SPIx发送一个byte数据
			
    while (spi_i2s_flag_get(spix, SPI_FLAG_RBNE) == RESET){;} //等待接收完一个byte  
	  return spi_i2s_data_receive(spix); //返回通过SPIx最近接收的数据		
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		取消片选，释放SPI总线
// @param		void     	
// @return	void      
// Sample usage:			    SD_DisSelect()
//                           //取消片选
//-------------------------------------------------------------------------------------------------------------------	
//取消片选 释放SPI总线
void SD_DisSelect(void)
{
    spi_nss_internal_high(spix);
    SPI_ReadWrite_Byte(0xff);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		获得响应
// @param		Response     	 期望响应值		
// @return	uint8_t       返回响应是否正确
// Sample usage:			    SD_GetResponse(0xfe)
//                           //发送0xff，判断SD卡响应值是否符合预期
//-------------------------------------------------------------------------------------------------------------------	
uint8_t SD_GetResponse(uint8_t Response)
{
    uint16_t Count=0xFFFF;   		//等待次数		
	
    while((SPI_ReadWrite_Byte(0xff)!=Response)&&Count)
		{
		    Count--;
		}
		printf(".");                                        //使用DMA后无此句则响应错误？？？
		
    if (Count==0)
		{
		    return MSD_RESPONSE_FAILURE;
		}  
    else 
    {
		    return MSD_RESPONSE_NO_ERROR;
		}
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		初始化SD卡（可尝试更改SPI频率？）
// @param		void     	   		
// @return	uint32_t       初始化成功返回SD卡扇数量；错误则返回其他值
// Sample usage:			    SD_Init_Config();
//                           //初始化SD卡
//-------------------------------------------------------------------------------------------------------------------	
uint32_t SD_Init_Config(void)
{
    uint8_t r1; //存储命令响应值 
    uint16_t retry, i; 
    uint8_t buf[4];  //存储CMD8命令的R7格式响应后四位
 
    SPI_init();	//初始化IO
    for(i=0;i<10;i++)
	  {
		    SPI_ReadWrite_Byte(0XFF);//发送至少74个脉冲
	  }
		
		retry=20;
    do
    {
	      r1=SD_SendCmd(CMD0,0,0x95);//进入IDLE状态
    }while((r1!=0X01) && retry--);
    SD_Type=0;//默认无卡
		
    if(r1==0X01)
    {
	      if(SD_SendCmd(CMD8,0x1AA,0x87) == 1)//SD V2.0
	      {
		        for(i=0;i<4;i++)
					  {
						    buf[i]=SPI_ReadWrite_Byte(0XFF);	
					  }//Get trailing return value of R7 resp
		        if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
		        {
		            retry=0XFFFE;
		            do
		            {
			              SD_SendCmd(CMD55,0,0X01);	//发送CMD55
			              r1=SD_SendCmd(CMD41,0x40000000,0X01);//发送CMD41
		            }while(r1&&retry--);
		            if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0开始
		            {
			              for(i=0;i<4;i++)
									  {
									      buf[i]=SPI_ReadWrite_Byte(0XFF);
									  }//得到OCR值
			              if(buf[0]&0x40)
										{
											  SD_Type=SD_TYPE_V2HC;
										}//检测CCS
			              else 
										{
											  SD_Type=SD_TYPE_V2; 
										}  
		            }
		        }
		     }
				
				 else//SD V1.x/ MMC	V3
		     {
		         SD_SendCmd(CMD55,0,0X01);//发送CMD55
		         r1=SD_SendCmd(CMD41,0,0X01);	//发送CMD41
		         if(r1<=1)
		         {		
			           SD_Type=SD_TYPE_V1;
			           retry=0XFFFE;
			           do 
			           {
			               SD_SendCmd(CMD55,0,0X01);	//发送CMD55
                     r1=SD_SendCmd(CMD41,0,0X01);//发送CMD41
			           }while(r1&&retry--);
		         }
						 else
		         {
			           SD_Type=SD_TYPE_MMC;//MMC V3
			           retry=0XFFFE;
			           do 
			           {											    
				             r1=SD_SendCmd(CMD1,0,0X01);
			           }while(r1&&retry--);  
		         }
		         if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;
		     }
    }
    SD_DisSelect();

//	  sectors_num = SD_GetSectorCount(csd);//获取CSD信息
	 
//	 spi_parameter_struct spi_init_struct;///////////////////////*
//	 
//	 if((csd[12]&0x07) == 0x03)
//	 {
//		 spi_init_struct.prescale = SPI_PSC_2;
//	 }
//	 else if((csd[12]&0x07) == 0x02)
//	 {
//		 spi_init_struct.prescale   = SPI_PSC_64;
//	 }
//   spi_init(spix, &spi_init_struct);
	 
    if(SD_Type)
		{
			return sectors_num;
    }
		else if(r1)
		{
				return r1;
		}				
    return 0xaa;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		从SD卡读取一个扇区的内容
// @param		buf     	    数据存储的地址		
// @param		arg     	    包含扇区号的命令参数	
// @param		len     	    读取字节长度len
// @return	uint8_t       读取成功返回0
// Sample usage:			    SD_RecvData(buf, 0x00, 3);
//                           //读取3个字节的内容存储到buf
//-------------------------------------------------------------------------------------------------------------------	
uint8_t SD_RecvData(uint8_t* buf, uint32_t arg, uint16_t len)
{			  	  	
	  spi_nss_internal_low(spix);
	  while(SD_GetResponse(0xff)){;}
    while(SD_SendCmd(CMD17, arg, 0xff) != 0x00){;}
	  while(SD_GetResponse(0xfe)){;}
    while(len--)
    {
        *buf=SPI_ReadWrite_Byte(0xff);
        buf++;
    }
    while(SPI_ReadWrite_Byte(0xff) == 0xff){;}
    SD_DisSelect();
		
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		从SD卡写入一个扇区的内容
// @param		buf     	    数据缓存的地址		
// @param		arg     	    包含扇区号的命令参数	
// @return	uint8_t       返回0成功；1失败
// Sample usage:			    SD_SendBlock(buffer, 0x00);
//                           //一个扇区大小的数据（首地址为buffer）写入SD卡
//-------------------------------------------------------------------------------------------------------------------	
uint8_t SD_SendBlock(uint8_t*buf, uint32_t arg)
{	
  uint16_t t;		  	  
	
	  spi_nss_internal_low(spix);
	
	  erasure(arg, arg);
	
	  while(SD_GetResponse(0xff)){;}
	  while(SD_SendCmd(CMD24, arg, 0xff) != 0x00){;}
	  SPI_ReadWrite_Byte(0xff);
	  SPI_ReadWrite_Byte(0xfe);
	  for(t=0;t<512;t++)
	  {
		    SPI_ReadWrite_Byte(buf[t]);
	  }
	  SPI_ReadWrite_Byte(0xFF);
	  SPI_ReadWrite_Byte(0xFF);
	  if((SPI_ReadWrite_Byte(0xFF)&0x1F)!=0x05)
		{
			  return 1;
		}		
    while(SPI_ReadWrite_Byte(0xff) == 0xff){;}	
	  SD_DisSelect();
		
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		扇区擦除（擦除某次保存数据首扇区，无法读取后续扇区数据？）
// @param		start_add     擦除起始扇区号
// @param		end_add     	擦除终止扇区号
// @return	void     
// Sample usage:			    erasure(1, 2)
//                           //擦除扇区1、2的数据
//-------------------------------------------------------------------------------------------------------------------	
void erasure(uint32_t start_add, uint32_t end_add)
{
	  spi_nss_internal_low(spix);
	  while(SD_GetResponse(0xff)){;}
	  while(SD_SendCmd(CMD32, start_add, 0xff) != 0x00){;}
	  while(SD_GetResponse(0xff)){;}
	  while(SD_SendCmd(CMD33, end_add, 0xff) != 0x00){;}
	  while(SD_GetResponse(0xff)){;}
	  while(SD_SendCmd(CMD38, 0, 0xff) != 0x00){;}
	  while(SPI_ReadWrite_Byte(0xff) == 0xff){;}	
	  SD_DisSelect();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		从SD卡读取多个扇区的内容
// @param		buf     	    数据存储的地址		
// @param		arg     	    包含起始扇区号的命令参数	
// @return	uint8_t       返回0成功；1读取地址越界
// Sample usage:			    SD_ReadDisk2(bufr, 0x01, 1200);
//                           //从扇区1开始读取1200字节的内容存储到bufr中
//-------------------------------------------------------------------------------------------------------------------	
uint8_t SD_ReadDisk2(uint8_t* buf,uint32_t sector, uint16_t len)
{
    uint16_t r1,num,count;

	  r1 = sector;
	  num = len/sector_size;
//	printf("page:");
	  count = len%sector_size;

	  spi_nss_internal_low(spix);
	  while(SD_GetResponse(0xff)){;}
    while(SD_SendCmd(CMD18, sector, 0xff) != 0x00){;}
	  for( ; num>0; num--)
	  {
	      while(SD_GetResponse(0xfe)){;}
        for(i=0; i<sector_size; i++)
        {
	     	    *buf=SPI_ReadWrite_Byte(0xff);
            buf++;
        }
        SPI_ReadWrite_Byte(0xff);
	      SPI_ReadWrite_Byte(0xff);

	      if((++r1) >= sectors_num) return 1;
	  }
    
		if(count > 0)
		{
        while(SD_GetResponse(0xfe)){;}
		    for(r1=0; r1<count; r1++)
        {
	          *buf=SPI_ReadWrite_Byte(0xff);
            buf++;
        }
	      SPI_ReadWrite_Byte(0xff);
	      SPI_ReadWrite_Byte(0xff);
	  }
	  SD_SendCmd(CMD12,0x00,0Xff);
    SD_DisSelect();
	
    return 0;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		从SD卡写入多个扇区的内容（使用DMA传输，注释部分为常规多页写入）
// @param		buf     	    数据缓存的起始地址				
// @param		arg     	    包含起始扇区号的命令参数	
// @return	uint8_t       返回0成功；1读取地址越界
// Sample usage:			    SD_WriteDisk2(buffr,0x01,1200)；
//                           //从扇区1开始写入buffr的1200字节
//-------------------------------------------------------------------------------------------------------------------	                                             
uint8_t SD_WriteDisk2(uint8_t*buf,uint32_t sector,uint16_t len)
{
	  uint16_t r1, num;//, i;
	
    r1 = sector;
	  num = ceil((float)len/sector_size);//写入总扇区数
	
	  spi_nss_internal_low(spix);
	
	  erasure(sector, sector+num-1);//擦除即将写入的扇区，提高写入速度
	
	  while(SD_GetResponse(0xff)){;}
    while(SD_SendCmd(CMD25, sector, 0xff) != 0x00){;}
	  SPI_ReadWrite_Byte(0xff);
	  SPI_ReadWrite_Byte(0xff);

	  for( ; num>0; num--)
	  {
		    SPI_ReadWrite_Byte(0xfc);

	      dma_channel_enable(DMA1, channelx);
		    while(!dma_flag_get(DMA1, channelx,DMA_FLAG_FTF)){;}
		    dma_flag_clear(DMA1, channelx,DMA_FLAG_FTF);
        dma_channel_disable(DMA1, channelx);	
		
//		  for(i=0; i<sector_size; i++)
//	    {
//         SPI_ReadWrite_Byte(buf[r1*sector_size + i]);
//      }
			
	      SPI_ReadWrite_Byte(0xff);
	      SPI_ReadWrite_Byte(0xff);
	      while((SPI_ReadWrite_Byte(0xFF)&0x1F)!=0x05){;}
	      while((SD_GetResponse(0xFF))){;}
		
		    if((++r1) >= sectors_num) return 1;//判断是否越界
		    buf += sector_size;//改变下一次DMA传输数据首地址
	      dma_memory_address_config(DMA1, channelx, DMA_MEMORY_0, (uint32_t)buf);//buf地址存储在DMA_MEMORY_0中
    }
	
	  SPI_ReadWrite_Byte(0xfd);
	  while((SD_GetResponse(0xFF))){;}
	  SD_DisSelect();
	
    return num;
}
