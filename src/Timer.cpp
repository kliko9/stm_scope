#include "stm32f4xx.h"
#include "core_cm4.h"
#include "STick.h"

#include "Timer.h"

namespace util {

Timer::Timer(TimerCb cb, unsigned time, void *data, bool renew)
	: cb_(cb), time_(time), data_(data), renew_(renew)
{
	STick::Instance().RegisterCallback(this);
}

Timer::~Timer() {
	STick::Instance().UnregisterCallback(this);
}

}

