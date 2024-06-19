#ifndef USART_H_
#define USART_H_

#include 	<stdint.h>
#include    <stdio.h>

#include 	"stm32f4xx.h"

void usart1_putchar(int ch);
uint32_t init_USART1(void);
char usart1_read(void);
void init_USART1_interrupt(void);

#endif /* USART_H_ */
