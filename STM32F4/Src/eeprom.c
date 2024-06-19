
/*
 * eeprom driver for stm32f4xx
 * by ihm3@
 *
 *
 * */

#include "stm32f4xx.h"
#include "eeprom.h"


uint32_t Flash_Unlock()
{
	uint32_t status = 0;

	if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
	{

		WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		WRITE_REG(FLASH->KEYR, FLASH_KEY2);


		if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
		{
			status = 1;
		}
	}

	return status;
}

void Flash_Write(uint32_t Address, uint32_t Data)
{
  CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
  FLASH->CR |= FLASH_PSIZE_WORD;
  FLASH->CR |= FLASH_CR_PG;

  *(__IO uint32_t*)Address = Data;
}

void eeprom_write(uint8_t* buffer, uint32_t addrs, uint32_t len)
{
	Flash_Unlock();

	//for(uint32_t i = 0; i < len; i++)
		//Flash_Write(addrs + i, buffer[i]);
}
