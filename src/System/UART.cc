#include <string>
#include "System/UART.h"

namespace sys {

extern "C" {
void USART1_IRQHandler()
{
	if (USART1->SR & USART_SR_RXNE) {
		char data = (USART1->DR & 0xFF);

		UART::Instance().AppendBuffer(data);
		//if (data == '\0')
			for (auto &cb: UART::Instance().Receivers())
				cb(UART::Instance().Buffer());

	} else if (USART1->SR & USART_SR_TXE) {
		auto setter = UART::Instance().Setter();
		if (setter)
			USART1->DR = setter();
		else {
			USART1->CR1 &= ~USART_CR1_TXEIE;
			USART1->CR1 &= ~USART_CR1_TE;
		}
	}
}
}

UART &UART::Instance()
{
	static UART instance;
	return instance;
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
	//USART1->BRR = 0x222E;				//set baudrate 9600
	USART1->BRR = 0x0000002D9;			//set baudrate 115226
	//USART1->CR2 |= (2UL << );			//set number of stop bits to 2
	//USART1->CR1 |= (1UL << 12);			//set mantysa, 0 is by default which is 8 bits
	//USART1->CR3 |= (DMAT dma enable for now not);	//enable dma
	//USART1->CR1 |= USART_CR1_RXNEIE;

	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
}

void UART::BeginTransmission()
{
	USART1->CR1 |= USART_CR1_TXEIE | USART_CR1_TE;
}

void UART::StopTransmission()
{
	USART1->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TE);
}

void UART::RegisterDataSetter(const std::function<char()> &fn)
{
	setter_ = fn;
}

void UART::RegisterDataReceiver(const std::function<void(const char *)> &fn)
{
	USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_RE;
	receivers_.push_back(fn);
}

void UART::SendCmd(const char *cmd, unsigned length)
{
	cmd_ = cmd;
	cmd_len_ = length;

	RegisterDataSetter(
			[this]()-> char {
				return WriteCmd();
			}
		);

	BeginTransmission();
}

char UART::WriteCmd()
{
	static unsigned idx;

	if (!cmd_ || !cmd_len_)
		return 0;

	if (idx > cmd_len_) {
		setter_ = nullptr;
		cmd_ = nullptr;
		cmd_len_ = 0;
		idx = 0;

		return 0;
	}

	return cmd_[idx++];
}

void UART::AppendBuffer(char s)
{
	if (buffer_idx_ > RECV_MAX_LENGTH - 2) {
		buffer_[RECV_MAX_LENGTH - 1] = '\0';
		buffer_idx_ = 0;
	}

	buffer_[buffer_idx_++] = s;
	if (buffer_[buffer_idx_ - 1] == 0 || buffer_[buffer_idx_ - 1] == '\n')
		buffer_idx_ = 0;
}

} // system
