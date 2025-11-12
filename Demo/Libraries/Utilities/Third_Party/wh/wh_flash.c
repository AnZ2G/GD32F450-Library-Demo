/*********************************************************************************************************************
* @file				wh_flash.c
* @author			Andreas WH
* @version			2.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

#include "wh_flash.h"

//-------------------------------------------------------------------------------------------------------------------
//	@brief			通过扇区name获取扇区number
//	@param			扇区 name
//	@return			扇区 number
//	Sample usage：
//-------------------------------------------------------------------------------------------------------------------
uint32_t flash_getSectorNumber(uint32_t sector_name)
{
    if (11 >= sector_name) {
        return CTL_SN(sector_name);
    } else if (23 >= sector_name) {
        return CTL_SN(sector_name + 4);
    } else if (27 >= sector_name) {
        return CTL_SN(sector_name - 12);
    } else {
		return FMC_WRONG_SECTOR_NAME;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//	@brief			通过扇区 name 获取扇区起始地址
//	@param			扇区 name
//	@return			扇区起始地址
//	Sample usage：
//-------------------------------------------------------------------------------------------------------------------
uint32_t flash_getSectorStartAddress(uint32_t sectorName)
{
    if (sectorName < 4) {
        return FMC_BANK0_START_ADDRESS + sectorName * SIZE_16KB;
    } else if (sectorName < 5) {
        return FMC_BANK0_START_ADDRESS + 4 * SIZE_16KB;
    } else if (sectorName < 12) {
        return FMC_BANK0_START_ADDRESS + 4 * SIZE_16KB + SIZE_64KB + (sectorName - 5) * SIZE_128KB;
    } else if (sectorName < 16) {
        return FMC_BANK1_START_ADDRESS + (sectorName - 12) * SIZE_16KB;
    } else if (sectorName < 17) {
        return FMC_BANK1_START_ADDRESS + 4 * SIZE_16KB;
    } else if (sectorName < 24) {
        return FMC_BANK1_START_ADDRESS + 4 * SIZE_16KB + SIZE_64KB + (sectorName - 17) * SIZE_128KB;
    } else if (sectorName < 28) {
        return FMC_BANK1_START_ADDRESS + 4 * SIZE_16KB + SIZE_64KB + 7 * SIZE_128KB + (sectorName - 24) * SIZE_256KB;
    } else {
        return FMC_INVALID_ADDR;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//	@brief			写 uint32_t 数据
//	@param			扇区 name
//	@param			偏移地址
//	@param			要写入的数据
//	@param			要写入的数据长度
//	@return			返回-1为失败，1为成功
//	Sample usage：	
//-------------------------------------------------------------------------------------------------------------------
int flash_WriteSector_uint32_t(uint32_t sectorName,
    uint32_t offset,
    uint32_t* writeData,
    uint16_t length)
{
    uint32_t sector_num, i, sector_start_address, sector_end_address;

    /* get the address of the start and end sector*/
    sector_start_address = offset + flash_getSectorStartAddress(sectorName);
    sector_end_address = sector_start_address + 4 * length;

    //avoid too large
    if ((sector_end_address - sector_start_address) >= SIZE_16KB) {
        return -1;
    }

    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    /* erase sector */
    sector_num = flash_getSectorNumber(sectorName);
    while (FMC_READY != fmc_sector_erase(sector_num))
        ;

    /* write data_32 to the corresponding address */

    for (i = 0; i < length; i++) {
        while (FMC_READY != fmc_word_program(sector_start_address, writeData[i]))
            ;
        sector_start_address = sector_start_address + 4;
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//	@brief			读 uint32_t 数据
//	@param			扇区 name
//	@param			偏移地址
//	@param			读取的数据
//	@param			要读取的数据长度
//	@return			返回-1为失败，1为成功
//	Sample usage：	
//-------------------------------------------------------------------------------------------------------------------
int flash_ReadSector_uint32_t(uint32_t sectorName,
    uint32_t offset,
    uint32_t* readData,
    uint16_t length)
{
    uint32_t i, sector_start_address, sector_end_address;
    /* get the address of the start and end sector*/
    sector_start_address = offset + flash_getSectorStartAddress(sectorName);
    sector_end_address = sector_start_address + 4 * length;

    //avoid too large
    if ((sector_end_address - sector_start_address) >= SIZE_16KB) {
        return -1;
    }

    for (i = 0; i < length; i++) {
        readData[i] = *(__IO int32_t*)sector_start_address;

        sector_start_address = sector_start_address + 4;
    }
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//	@brief			写 uint16_t 数据
//	@param			扇区 name
//	@param			偏移地址
//	@param			要写入的数据
//	@param			要写入的数据长度
//	@return			返回-1为失败，1为成功
//	Sample usage：	
//-------------------------------------------------------------------------------------------------------------------
int flash_WriteSector_uint16_t(uint32_t sectorName,
    uint32_t offset,
    uint16_t* writeData,
    uint16_t length)
{
    uint32_t sector_num, i, sector_start_address, sector_end_address;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* get the address of the start and end sector*/
    sector_start_address = offset + flash_getSectorStartAddress(sectorName);
    sector_end_address = sector_start_address + 2 * length;

    //avoid too large
    if ((sector_end_address - sector_start_address) >= SIZE_16KB) {
        return -1;
    }

    /* erase sector */
    sector_num = flash_getSectorNumber(sectorName);
    while (FMC_READY != fmc_sector_erase(sector_num))
        ;

    /* write data to the corresponding address */

    for (i = 0; i < length; i++) {
        while (FMC_READY != fmc_halfword_program(sector_start_address, writeData[i]))
            ;
        sector_start_address = sector_start_address + 2;
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//	@brief			读 uint16_t 数据
//	@param			扇区 name
//	@param			偏移地址
//	@param			读取的数据
//	@param			要读取的数据长度
//	@return			返回-1为失败，1为成功
//	Sample usage：	
//-------------------------------------------------------------------------------------------------------------------
int flash_ReadSector_uint16_t(uint32_t sectorName,
    uint32_t offset,
    uint16_t* readData,
    uint16_t length)
{
    uint32_t i, sector_start_address, sector_end_address;
    /* get the address of the start and end sector*/
    sector_start_address = offset + flash_getSectorStartAddress(sectorName);
    sector_end_address = sector_start_address + 2 * length;

    //avoid too large
    if ((sector_end_address - sector_start_address) >= SIZE_16KB) {
        return -1;
    }

    for (i = 0; i < length; i++) {
        readData[i] = *(__IO int16_t*)sector_start_address;

        sector_start_address = sector_start_address + 2;
    }
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//	@brief			写 uint8_t 数据
//	@param			扇区 name
//	@param			偏移地址
//	@param			要写入的数据
//	@param			要写入的数据长度
//	@return			返回-1为失败，1为成功
//	Sample usage：	
//-------------------------------------------------------------------------------------------------------------------
int flash_WriteSector_uint8_t(uint32_t sectorName,
    uint32_t offset,
    uint8_t* writeData,
    uint16_t length)
{
    uint32_t sector_num, i, sector_start_address, sector_end_address;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* get the address of the start and end sector*/
    sector_start_address = offset + flash_getSectorStartAddress(sectorName);
    sector_end_address = sector_start_address + 1 * length;

    //avoid too large
    if ((sector_end_address - sector_start_address) >= SIZE_16KB) {
        return -1;
    }

    /* erase sector */
    sector_num = flash_getSectorNumber(sectorName);
    while (FMC_READY != fmc_sector_erase(sector_num))
        ;

    /* write data to the corresponding address */

    for (i = 0; i < length; i++) {
        while (FMC_READY != fmc_byte_program(sector_start_address, writeData[i]))
            ;
        sector_start_address = sector_start_address + 1;
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//	@brief			读 uint8_t 数据
//	@param			扇区 name
//	@param			偏移地址
//	@param			读取的数据
//	@param			要读取的数据长度
//	@return			返回-1为失败，1为成功
//	Sample usage：	
//-------------------------------------------------------------------------------------------------------------------
int flash_ReadSector_uint8_t(uint32_t sectorName,
    uint32_t offset,
    uint8_t* readData,
    uint16_t length)
{
    uint32_t i, sector_start_address, sector_end_address;
    /* get the address of the start and end sector*/
    sector_start_address = offset + flash_getSectorStartAddress(sectorName);
    sector_end_address = sector_start_address + 1 * length;

    //avoid too large
    if ((sector_end_address - sector_start_address) >= SIZE_16KB) {
        return -1;
    }

    for (i = 0; i < length; i++) {
        readData[i] = *(__IO int8_t*)sector_start_address;

        sector_start_address = sector_start_address + 1;
    }
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//	@brief			数组数据转移
//	@param			trans_buffer		发送的数组
//	@param			rev_buffer			接收的数组
//	@param			trans_start_num		发送数组的起始下标
//	@param			rev_start_num		接收数组的起始下标
//	@param			len					接收长度
//	@return			void
//	Sample usage：	
//-------------------------------------------------------------------------------------------------------------------
void flash_buffer_translate(uint8_t* trans_buffer, uint8_t* rev_buffer, uint16_t trans_start_num, uint16_t rev_start_num, uint16_t len)
{
    uint16_t start,end;
    start = trans_start_num + len;
    end = rev_start_num + len;
    while(len)
    {
        rev_buffer[end - len] = trans_buffer[start - len];
        len--;
    }
}
