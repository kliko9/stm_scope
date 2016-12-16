#include "stm32f4xx.h"
#include <functional>

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

void TimerInit() {

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->CR1 |= (1 << 2) | (1 << 4) | (1 << 1);
	TIM1->PSC = SystemCoreClock / 1000000 - 1;	//Set prescaler so timer's frequency is 1MHz
	TIM1->ARR = 1000;
	TIM1->RCR = 50;

	/*
	TIM->RCR+1 <= sets repetition counter, if counting finishes UEV update event is generated
	check UIF register
	*/

	TIM1->CR1 |= 1;
}

void TestCb(void *data) {

	GPIOD->ODR ^= ((uint32_t)1<<15);
}

void Test2Cb(void *data) {

}

void MainInit() {

	GPIOInit();
	TimerInit();
}

int main(void)
{
	MainInit();

	utils::Timer tim1(TestCb, 1000, nullptr);

	sys::ADConverter adc;

	sys::UART uart;

	std::function<char()> fn = [&]()-> char {
		return adc.CurrentValue();
	};

	uart.RegisterDataGetter(fn);

	uart.BeginTransmission();

	while(1) {
	}
}
