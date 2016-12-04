#pragma once

namespace system {

class ADConverter {
public:
	ADConverter();
	virtual ~ADConverter();

private:
	void GPIOInit();
	void ADCInit();

};

} // system
