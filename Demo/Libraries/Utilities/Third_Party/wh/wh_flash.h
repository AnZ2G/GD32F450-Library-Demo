/*********************************************************************************************************************
* @file				wh_flash.h
* @author			Andreas WH
* @version			2.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

#ifndef _WH_FLASH_
#define _WH_FLASH_

#include "gd32f4xx.h"

/* 扇区大小定义 */
#define SIZE_16KB ((uint32_t)0x00004000U) /*!< size of 16KB*/
#define SIZE_64KB ((uint32_t)0x00010000U) /*!< size of 64KB*/
#define SIZE_128KB ((uint32_t)0x00020000U) /*!< size of 128KB*/
#define SIZE_256KB ((uint32_t)0x00040000U) /*!< size of 256KB*/

/* FMC Flash 地址定义 */
#define FMC_START_ADDRESS FLASH_BASE /*!< FMC start address */
#define FMC_BANK0_START_ADDRESS \
    FMC_START_ADDRESS /*!< FMC BANK0 start address */
#define FMC_BANK1_START_ADDRESS \
    ((uint32_t)0x08100000U) /*!< FMC BANK1 start address */
#define FMC_SIZE (*(uint16_t*)0x1FFF7A22U) /*!< FMC SIZE */
#define FMC_END_ADDRESS \
    (FLASH_BASE + (FMC_SIZE * 1024) - 1) /*!< FMC end address */
#define FMC_MAX_END_ADDRESS \
    ((uint32_t)0x08300000U) /*!< FMC maximum end address */

/* FMC 错误信息定义 */
#define FMC_WRONG_SECTOR_NAME ((uint32_t)0xFFFFFFFFU) /*!< wrong sector name*/
#define FMC_WRONG_SECTOR_NUM \
    ((uint32_t)0xFFFFFFFFU) /*!< wrong sector number*/
#define FMC_INVALID_SIZE ((uint32_t)0xFFFFFFFFU) /*!< invalid sector size*/
#define FMC_INVALID_ADDR ((uint32_t)0xFFFFFFFFU) /*!< invalid sector address*/

// 通过 扇区名 获取 扇区号
uint32_t flash_getSectorNumber(uint32_t sector_name);

// 通过 扇区名 获取 扇区起始地址
uint32_t flash_getSectorStartAddress(uint32_t sectorName);

// 向指定扇区写入 uin32_t 类型数据
int flash_WriteSector_uint32_t(uint32_t sectorName,
    uint32_t offset,
    uint32_t* writeData,
    uint16_t length);

// 从指定扇区读取 uin32_t 类型数据
int flash_ReadSector_uint32_t(uint32_t sectorName,
    uint32_t offset,
    uint32_t* readData,
    uint16_t length);

// 向指定扇区写入 uin16_t 类型数据
int flash_WriteSector_uint16_t(uint32_t sectorName,
    uint32_t offset,
    uint16_t* writeData,
    uint16_t length);

// 从指定扇区读取 uin16_t 类型数据
int flash_ReadSector_uint16_t(uint32_t sectorName,
    uint32_t offset,
    uint16_t* readData,
    uint16_t length);

// 向指定扇区写入 uin8_t 类型数据
int flash_WriteSector_uint8_t(uint32_t sectorName,
    uint32_t offset,
    uint8_t* writeData,
    uint16_t length);

// 从指定扇区读取 uin8_t 类型数据
int flash_ReadSector_uint8_t(uint32_t sectorName,
    uint32_t offset,
    uint8_t* readData,
    uint16_t length);

void flash_buffer_translate(uint8_t* trans_buffer, uint8_t* rev_buffer, uint16_t trans_start_num, uint16_t rev_start_num, uint16_t len);
#endif
