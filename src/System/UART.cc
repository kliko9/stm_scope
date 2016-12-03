#include "System/UART.h"

namespace system {

extern "C" {
void USART1_IRQHandler()
{
	//while ((USART1->SR & 1 << 7) == 0) {};
	if ((USART1->SR & 1 << 7) == 0)
		return;

	USART1->DR = 'A';
}
}

UART::UART()
{
	GPIOInit();
	UARTInit();
}

UART::~UART()
{
}

void UART::GPIOInit()
{
	/* USART1
	 * Pb6	TX
	 * PA7	RX
	 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->OSPEEDR |= (2UL << 12) | (2UL << 14);	//set 50 MHz speed
	GPIOB->MODER |= (2UL << 12) | (2UL << 14);	//set to alternate function
	GPIOB->AFR[0] |= (7UL << 24) | (7UL << 28);	//choose USART1 as AF
	GPIOB->PUPDR |= (1UL << 12) | (1UL << 14);
}

void UART::UARTInit()
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/* Set baudrate to 9600
	 * Programmed value with 84Mhz is 1093.75
	 * Fraction: 16 * 0.75 = 12 = 0xC
	 * Mantissa: 1093 = 0x445
	 */

	USART1->CR1 |= USART_CR1_UE;	//enable uart and interrupt on data shift
	//USART1->BRR = 0x445C;				//set baudrate 9600
	USART1->BRR = 0x0000002D9;			//set baudrate 115226
	USART1->CR2 |= USART_CR2_STOP_0;		//set number of stop bits to 2
	//USART1->CR1 &= ~(1 << );			//set mantysa, 0 is by default which is 8 bits
	//USART1->CR3 |= (DMAT dma enable for now not);	//enable dma

	USART1->CR1 |= USART_CR1_TE | USART_CR1_TXEIE;

	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
}

void UART::SendString(const char *string)
{
	while (*string++) {
		SendData(*string);
	}
}

void UART::SendData(const uint8_t &data)
{
	USART1->DR = data;
}

} // system
