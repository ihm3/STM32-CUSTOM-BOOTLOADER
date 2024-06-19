/*
 * uart.c
 *

 *      Author: ihm3
 */


#include "uart.h"

#define GPIOAEN		(1U<<0)
#define UART1EN		(1U<<13)

#define FREQ		16000000
#define AHB2_CLK	FREQ
#define BOUDRATE 	115200

#define CR1_RXNEIE	(1U<<5)

static void Set_Boudrate(USART_TypeDef* USARTx, uint32_t clk, uint32_t boudrate);

int __io_putchar(int chr)//printf kullanımı için
{
	usart1_putchar(chr);
	return chr;
}

char usart1_read(void)
{
	while(!(USART1->SR & (1U<<5))){}
	return USART1->DR;
}

void usart1_putchar(int ch)
{
	/*Transmit data registerı bos olup olmadığı kontrol edilmeli*/
	while(!(USART1->SR & (1U<<7))){}

	/*DATA transmit data registerına yazılır*/
	USART1->DR = (ch & 0xFF);
}

uint32_t init_USART1(void)//USART 1 kullanılacak : PA9&PA10
{
	/**************USART PIN AYARLARI******************/
	//1-GPIO clock erısımı yetkilendirilir
	RCC->AHB1ENR = GPIOAEN;

	//2-PA9 alternate function moduna ayarlanmalı
	GPIOA->MODER &=~ (1U<<18);
	GPIOA->MODER |=  (1U<<19);

	//PA10 alternate function moduna ayarlanmalı
	GPIOA->MODER &=~ (1U<<20);
	GPIOA->MODER |=  (1U<<21);

	//3-PA9 alternate function tipi ayarlanmalı(AF07) GPIOA_AFRH : 0111
	GPIOA->AFR[1] |=  (1U<<4);
	GPIOA->AFR[1] |=  (1U<<5);
	GPIOA->AFR[1] |=  (1U<<6);
	GPIOA->AFR[1] &=~ (1U<<7);

	//PA10 alternate function tipi ayarlanmalı(AF07) GPIOA_AFRH : 0111
	GPIOA->AFR[1] |=  (1U<<8);
	GPIOA->AFR[1] |=  (1U<<9);
	GPIOA->AFR[1] |=  (1U<<10);
	GPIOA->AFR[1] &=~ (1U<<11);

	/**************USART MODULE AYARLARI**************/
	//1- USART modulu clock erısımı yetkilendirilir
	RCC->APB2ENR |=   (1U<<4);

	//2- BOUD RATE ayarlanır
	Set_Boudrate(USART1, AHB2_CLK, BOUDRATE);

	//3- Transfer yonu secılır(TX)
	USART1->CR1 = ((1U<<3) | (1U<<2));// TE&RE biti harici tum bitler 0 olur(diğer bitler 0 da defoult değerler)


	//5- USART modulu yetkilendirilir
	USART1->CR1 |=  UART1EN;//sadece UE(13.bit) 1 yapılır

	return 0;
}

void init_USART1_interrupt(void)//USART 1 kullanılacak : PA9&PA10
{
	/**************USART PIN AYARLARI******************/
	//1-GPIO clock erısımı yetkilendirilir
	RCC->AHB1ENR = GPIOAEN;

	//2-PA9 alternate function moduna ayarlanmalı
	GPIOA->MODER &=~ (1U<<18);
	GPIOA->MODER |=  (1U<<19);

	//PA10 alternate function moduna ayarlanmalı
	GPIOA->MODER &=~ (1U<<20);
	GPIOA->MODER |=  (1U<<21);

	//3-PA9 alternate function tipi ayarlanmalı(AF07) GPIOA_AFRH : 0111
	GPIOA->AFR[1] |=  (1U<<4);
	GPIOA->AFR[1] |=  (1U<<5);
	GPIOA->AFR[1] |=  (1U<<6);
	GPIOA->AFR[1] &=~ (1U<<7);

	//PA10 alternate function tipi ayarlanmalı(AF07) GPIOA_AFRH : 0111
	GPIOA->AFR[1] |=  (1U<<8);
	GPIOA->AFR[1] |=  (1U<<9);
	GPIOA->AFR[1] |=  (1U<<10);
	GPIOA->AFR[1] &=~ (1U<<11);

	/**************USART MODULE AYARLARI**************/
	//1- USART modulu clock erısımı yetkilendirilir
	RCC->APB2ENR |=   (1U<<4);

	//2- BOUD RATE ayarlanır
	Set_Boudrate(USART1, AHB2_CLK, BOUDRATE);

	//3- Transfer yonu secılır(TX)
	USART1->CR1 = ((1U<<3) | (1U<<2));// TE&RE biti harici tum bitler 0 olur(diğer bitler 0 da default değerler)

	//RX interrupt biti setlenir
	USART1->CR1	|=	 CR1_RXNEIE;

	//NVIC te uart1 interruptı yetkilendirlir
	NVIC_EnableIRQ(USART1_IRQn);

	//5- USART modulu yetkilendirilir
	USART1->CR1 |=  UART1EN;//sadece UE(13.bit) 1 yapılır

}

static void Set_Boudrate(USART_TypeDef* USARTx, uint32_t clk, uint32_t boudrate)
{
	USARTx->BRR = ((clk + (boudrate/2U)) / boudrate);//BRR registerında boudrate ayarı
}
