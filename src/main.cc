#include "stm32f4xx.h"

#include <functional>
#include <cstring>

#include "Utils/Timer.h"
#include "System/UART.h"
#include "System/ADConverter.h"

void GPIOInit() {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	GPIOD->MODER |= ((uint32_t)1 << 30) | ((uint32_t) 1 << 28);
	GPIOD->OTYPER &= ~(((uint16_t)1 << 15) | ((uint16_t)1 << 14));
	GPIOD->OSPEEDR |= ((uint32_t)3 << 30) | ((uint32_t) 1 << 28);
	GPIOD->PUPDR |= ((uint32_t)1 << 30) | ((uint32_t) 1 << 28);
}

void RedLedToggle(void *data) {

	GPIOD->ODR ^= ((uint32_t)1<<14);
}

int main(void)
{
	GPIOInit();

	utils::Timer tim1(RedLedToggle, 1000, nullptr);

	sys::ADConverter adc;
	/* ECHO - device responds on received information by sending it back to source

	sys::UART::Instance().RegisterDataReceiver([](const char *data){
			sys::UART::Instance().SendCmd(data, strlen(data) + 1);
			}
			);
	*/

	//sys::UART::Instance().SendCmd(UART_CMD_SET_NAME, strlen(UART_CMD_SET_NAME));

	std::function<char()> fn = [&]()-> char {
		return adc.CurrentValue();
	};

	sys::UART::Instance().RegisterDataSetter(fn);
	sys::UART::Instance().BeginTransmission();

	while(1) {
	}
}
