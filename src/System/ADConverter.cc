#include "stm32f4xx.h"
#include "System/ADConverter.h"

namespace system {

extern "C" {
void ADC_IRQHandler()
{
	static int currentValue;

	if ((ADC1->SR & ADC_SR_EOC))
		currentValue = ADC1->DR;
}
}

ADConverter::ADConverter()
{
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
	GPIOB->MODER |= GPIO_MODER_MODER0;		//set to alternate function
}

void ADConverter::ADCInit()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	//ADC on, set continuous mode, enable end of conversion interruption
	ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;	//| ADC_CR2_DMA if necessary
	ADC1->SQR2 |= (1UL << 5);			//select channel8
	//ADC1->CR1 |= ADC_CR1_RES;			//resolutio is by default 12bit
	ADC1->CR1 |= ADC_CR1_EOCIE;
	ADC1->CR2 |= ADC_CR2_SWSTART;			//start conversion, set 0 to ADON to stop conversion

	NVIC_SetPriority(ADC_IRQn, 0);
	NVIC_EnableIRQ(ADC_IRQn);
}

} // system
