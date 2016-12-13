#pragma once

#include <functional>

#include "stm32f4xx.h"

namespace sys {

class UART {
public:
	UART();
	virtual ~UART();

	void RegisterDataGetter(std::function<char()> &fn);

	void BeginTransmission();
	void StopTransmission();

private:
	void GPIOInit();
	void UARTInit();
};

} // system
