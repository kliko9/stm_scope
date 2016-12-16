#include "stm32f4xx.h"
#include "System/ADConverter.h"

namespace sys {

char *dataBuffer;

extern "C" {
void ADC_IRQHandler()
{
	if (ADC1->SR & ADC_SR_EOC)
		*dataBuffer = (ADC1->DR & 0xFF);
}
}

ADConverter::ADConverter()
{
	dataBuffer = &value;

	GPIOInit();
	ADCInit();
}

ADConverter::~ADConverter()
{
	NVIC_DisableIRQ(ADC_IRQn);
}

void ADConverter::GPIOInit()
{
	/* Configuration for PB0 ADConverter12_IN8
	 *
	 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;	//the speeeeeeed!!
	GPIOB->MODER |= GPIO_MODER_MODER0;		//analog mode
}

void ADConverter::ADCInit()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	//ADC on, set continuous mode, enable end of conversion interruption
	ADC1->CR2 |= ADC_CR2_CONT;	//| ADC_CR2_DMA if necessary
	ADC1->CR1 |= ADC_CR1_EOCIE;
	ADC1->SQR3 |= 8ul;				//select channel8
	ADC1->CR1 |= ADC_CR1_RES_1;			//for now 8 bit, resolution is by default 12bit

	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;			//start conversion, set 0 to ADON to stop conversion

	NVIC_SetPriority(ADC_IRQn, 0);
	NVIC_EnableIRQ(ADC_IRQn);
}

char ADConverter::CurrentValue()
{
	return value;
}

} // system
