#pragma once

namespace sys {

class ADConverter {
public:
	ADConverter();
	virtual ~ADConverter();

	char CurrentValue();
private:
	char value = 0;

	void GPIOInit();
	void ADCInit();
};

} // system
