#pragma once

#include "stm32f4xx.h"

namespace system {

class UART {
public:
	UART();
	virtual ~UART();

	void SendData(const uint8_t &data);
	void SendString(const char *string);

private:

	void GPIOInit();
	void UARTInit();

};

} // system
