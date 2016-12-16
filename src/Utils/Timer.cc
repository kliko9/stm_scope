#include "System/STick.h"
#include "Utils/Timer.h"

namespace utils {

Timer::Timer(std::function<void(void*)> cb, unsigned time, void *data, bool renew)
	: cb_(cb), time_(time), data_(data), renew_(renew)
{
	sys::STick::Instance().RegisterCallback(this);
}

Timer::~Timer() {
	sys::STick::Instance().UnregisterCallback(this);
}

}

