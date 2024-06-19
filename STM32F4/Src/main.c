/**
 ******************************************************************************
	@Author: ihm3   -- İsmail Hakkı Metehan EROL 
 ******************************************************************************
 */
#include "stm32f4xx.h"
#include  "uart.h"
#include  "data.h"
#include  "main.h"
#include  "eeprom.h"

#define GPIOAEN		(1U<<0)


SLAVE slave;

_Bool uartflg = false;
uint8_t index_ = 0;

uint32_t debug = 0;

int main(void)
{
	RCC->AHB1ENR |= GPIOAEN;//PA0 INPUT olarak ayarlanır boot modu kontrol edecek pin
	GPIOA->MODER &=~ (1U<<0);
	GPIOA->MODER &=~ (1U<<1);

	init_USART1_interrupt();// uart rx/tx ve rx interrupt configurasyonu(kendi driver ımı kullandım)


	bootcheck(); // boot modda baslatılıp baslatılmayacagını berlırleyecek fonks.


    return 0;
}

void bootcheck()
{
	if((GPIOA->IDR & (1U<<0)))// acılısta tanımlanan butoona basılırsa boot modda calısır
	{
		jump_to_user_app();// kullanıcı uygulamasına (vektor tablosuda dahıl) atlayacak fonks.
	}
	else
	{
		bootmode();// boot modda calısacak fonsıyon
	}
}

void bootmode()
{
	printf("Boot MODE\n");

	Flash_Unlock();

	while(1)
	 {
		 if(uartflg)
		 {
			 ErrorCode err = parser();
			 if(err == NO_ERROR)
				 data_handle();
			 else
				 printf("[E] : %d", err);

			 uartflg = false;
		 }
	 }
}

void jump_to_user_app()
{
	printf("User App\n");

	void (*user_app_reset_handler)(void); // kullanıcı (ana) uygulamanın reset handler ını tutan fonks. pointer ı

	/*MSP nin kullanıcı uygulamasına gore yapılandırtılması -- bu ugluma ıcın baslangıc adresi sector2 olarak secılmıstır*/
	uint32_t msp_value = *(volatile uint32_t *)SECTOR2_BASE_ADDR;
	printf("MSP Value %x\n", msp_value);

	//MSP yı arm cekirdegınde ayarlanması gerekır -- fonks. cmsis ten geliyor
	__set_MSP(msp_value);

	//program sector2 den baslatılır
	uint32_t resethandler_addr = *(volatile uint32_t *) (SECTOR2_BASE_ADDR + 4);

	user_app_reset_handler = (void *) resethandler_addr;

	user_app_reset_handler();
}

void USART1_IRQHandler(void) // recieve edilen her byte icin  interrupt alıcak
{
	//SR da RXNE  kontrolu
	uint32_t status = USART1->SR;

	if(status & (1U<<5))
	{

		//memcpy(&slave.COMMAND[index_], &USART1->DR, 1);// recv. edilen byte tanımlanan struct a yazılıyor


		if(index_  < sizeof(slave.COMMAND)) // mesaj uzunlugu kadar byte donduruldukten sonra  index basa doner ve ana programdakı  bayrak acılır
		{
			slave.COMMAND[index_] = USART1->DR;
			index_++;
			index_  == sizeof(slave.COMMAND) ? uartflg = true, index_ = 0 : index_;

		}
		else
		{
			index_++;
		}

	}
}

ErrorCode parser()
{
	ErrorCode ret;

	if(slave.PARS.start != 163/*£ = 163 (non ascii)*/) // mesaj baslangıc kontrolu
	{
		ret = INVALID_START_CHAR;

	}
	else if(slave.PARS.end != 37/*% = 37 ( ascii)*/) // mesaj bitis kontrolu
	{
		ret = INVALID_END_CHAR;
	}
	else
	{
		ret = NO_ERROR;
	}
	return ret;
}


int data_handle() // hata yoksa komutu gerceklestıren fonks.
{
	static uint32_t ind_ = 0;
	static uint32_t addr = SECTOR2_BASE_ADDR;

	if(slave.PARS.commandType)
	{
		Flash_Unlock();
		//eeprom_write((uint8_t*)slave.PARS.data, addr + ind_, 4); // gelen veri user app baslangıc adresinden baslayarak yazılır -- kendi eeprom driver ımı kullandım
		Flash_Write(addr + ind_, slave.PARS.data);
		ind_+= 4;// 4 byte lık veri geldıgı icin
	}
	else
	{
		//bootlodera okujma komutu eklenebilir
	}
	return 0;
}
