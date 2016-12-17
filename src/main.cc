#include "stm32f4xx.h"

#include <functional>
#include <cstring>

#include "Utils/Timer.h"
#include "System/UART.h"
#include "System/ADConverter.h"

void GPIOInit() {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	GPIOD->MODER |= (1UL << 30) | (1UL << 28);
	GPIOD->OTYPER &= ~((1UL << 15) | (1UL << 14));
	GPIOD->OSPEEDR |= (3UL << 30) | (1UL << 28);
	GPIOD->PUPDR |= (1UL << 30) | (1UL << 28);
}

void RedLedToggle(void *data) {

	GPIOD->ODR ^= (1UL << 14);
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

	sys::UART::Instance().SendCmd(UART_CMD_SET_NAME, strlen(UART_CMD_SET_NAME));

	std::function<char()> fn = [&]()-> char {
		return adc.CurrentValue();
	};

	utils::Timer *adcTimer = new utils::Timer([&](void *data){
				sys::UART::Instance().RegisterDataSetter(fn);
				sys::UART::Instance().BeginTransmission();

				GPIOD->ODR ^= (1UL << 15);
				delete adcTimer;
			},
			1000,
			nullptr);

	while(1) {}
}
