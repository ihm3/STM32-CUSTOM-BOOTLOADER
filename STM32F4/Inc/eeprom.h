/*
 * eeprom.h
 *
 *      Author: ihm3@
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

#define FLASH_PSIZE_BYTE           0x00000000U
#define FLASH_PSIZE_HALF_WORD      0x00000100U
#define FLASH_PSIZE_WORD           0x00000200U
#define FLASH_PSIZE_DOUBLE_WORD    0x00000300U
#define CR_PSIZE_MASK              0xFFFFFCFFU

#define RDP_KEY                  ((uint16_t)0x00A5)
#define FLASH_KEY1               0x45670123U
#define FLASH_KEY2               0xCDEF89ABU
#define FLASH_OPT_KEY1           0x08192A3BU
#define FLASH_OPT_KEY2           0x4C5D6E7FU



uint32_t Flash_Unlock();
void Flash_Write(uint32_t Address, uint32_t Data);
void eeprom_write(uint8_t* buffer, uint32_t addrs, uint32_t len);

#endif /* EEPROM_H_ */
