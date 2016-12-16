#include <string>
#include "System/UART.h"

namespace sys {

std::function<char()> dataGetter;

extern "C" {
void USART1_IRQHandler()
{
	if (USART1->SR & USART_SR_RXNE) {

	} else if (USART1->SR & USART_SR_TXE) {
		//USART1->DR = ('a' & (uint16_t)0x01FF);
		char data = dataGetter();
		USART1->DR = data;
	}
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
	 * PB6	TX
	 * PB7	RX
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
	 * Programmed value with 84Mhz is 546.875
	 * Fraction: 16 * 0.875 = 14 = 0xE
	 * Mantissa: 546 = 0x222;
	 */

	USART1->CR1 |= USART_CR1_UE;			//enable uart and interrupt on data shift
	USART1->BRR = 0x222E;				//set baudrate 9600
	//USART1->BRR = 0x0000002D9;			//set baudrate 115226
	//USART1->CR2 |= (2UL << );			//set number of stop bits to 2
	//USART1->CR1 |= (1UL << 12);			//set mantysa, 0 is by default which is 8 bits
	//USART1->CR3 |= (DMAT dma enable for now not);	//enable dma
	//USART1->CR1 |= USART_CR1_RXNEIE;


	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
}

void UART::BeginTransmission()
{
	USART1->CR1 |= USART_CR1_TXEIE;
	USART1->CR1 |= USART_CR1_TE;

	while ((USART1->SR & 1 << 7) == 0) {};

	USART1->DR = ('a' & (uint16_t)0x01FF);
}

void UART::StopTransmission()
{
	USART1->CR1 &= ~USART_CR1_TXEIE;
	USART1->CR1 &= ~USART_CR1_TE;
}

void UART::RegisterDataGetter(std::function<char()> &fn)
{
	dataGetter = fn;
}

} // system
