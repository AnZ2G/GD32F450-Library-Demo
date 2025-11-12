#ifndef _TF_SPI_H
#define _TF_SPI_H

#include "gd32f4xx.h"

/*********************************************************************************************
uint8_t CMD0[6] = {0x40,0x00,0x00,0x00,0x00,0x95};
uint8_t CMD1[6] = {0x41,0x00,0x00,0x00,0x00,0xff};
uint8_t CMD8[6] = {0x48,0x00,0x00,0x01,0xaa,0x87};
uint8_t CMD17[6] = {0x51,0x00,0x00,0x00,0x00,0xff};
uint8_t CMD24[6] = {0x58,0x00,0x00,0x00,0x00,0xff};
uint8_t CMD32[6] = {0x60,0x00,0x00,0x00,0x00,0xff};
uint8_t CMD33[6] = {0x61,0x00,0x00,0x00,0x00,0xff};
uint8_t CMD38[6] = {0x66,0x00,0x00,0x00,0x00,0xff};
uint8_t CMD55[6] = {0x77,0x00,0x00,0x00,0x00,0xff};
uint8_t ACMD41[6] = {0x69,0x40,0x00,0x00,0x00,0xff};
uint8_t CMD58[6] = {0x7a,0x00,0x00,0x00,0x00,0xff};
**********************************************************************************************/

// SD卡类型定义
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD卡指令表	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ,SEND_IF_COND
#define CMD9    9       //命令9 ,读CSD数据
#define CMD10   10      //命令10,读CID数据
#define CMD12   12      //命令12,停止数据传输
#define CMD16   16      //命令16,设置SectorSize 返回0x00
#define CMD17   17      //命令17,读sector
#define CMD18   18      //命令18,读 Multi sector
#define CMD23   23      //命令23,设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24,写sector
#define CMD25   25      //命令25,写Multi sector
#define CMD32   32      //命令32,设置擦除首地址
#define CMD33   33      //命令33,设置擦除末地址
#define CMD38   38      //命令38,擦除所选范围内容
#define CMD41   41      //命令41,返回0x00
#define CMD55   55      //命令55,返回0x01
#define CMD58   58      //命令58,读OCR信息
#define CMD59   59      //命令59,使能/禁止CRC,应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标志字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

#define sector_size 512
#define sectors_num 54525952//扇区容量  26G

#define rcu_spix RCU_SPI3
#define spix SPI3
#define rcu_dmax RCU_DMA1
#define dmax DMA1
#define channelx DMA_CH1


void DMA_init(uint32_t* txbuffer, uint32_t num);
void SPI_init(void);
uint8_t SPI_ReadWrite_Byte(uint8_t TxData);
void SD_DisSelect(void);
//uint8_t SD_WaitReady(void);
uint8_t SD_GetResponse(uint8_t Response);
uint8_t SD_RecvData(uint8_t*buf, uint32_t arg, uint16_t len);
uint8_t SD_SendBlock(uint8_t*buf, uint32_t arg);
uint8_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t SD_GetCSD(uint8_t *csd_data);
uint32_t SD_GetSectorCount(uint8_t* csd);
uint32_t SD_Init_Config(void);
uint8_t SD_ReadDisk2(uint8_t*buf,uint32_t sector, uint16_t len);
uint8_t SD_WriteDisk2(uint8_t*buf,uint32_t sector,uint16_t len);
void erasure(uint32_t start_add, uint32_t end_add);
 
#endif 
