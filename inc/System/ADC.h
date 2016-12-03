#pragma once

namespace system {

class ADC {
public:
	ADC();
	virtual ~ADC();

private:
	void GPIOInit();
	void ADCInit();

};

} // system
