#include "stm32f4xx.h"
#include "core_cm4.h"

#include "Timer.h"

namespace util {

Timer::Timer(unsigned int time_ms, TimerCb cb)
: cb_(cb), time_ms_(time_ms) {

	if (!SysTickInit()) {
		throw;
	}
}

Timer::~Timer() {

	SysTickDeinit();
}

bool Timer::SysTickInit() {

	if (SysTick_Config(SystemCoreClock/1000)) {
		return false;
	}

	return true;
}

void Timer::SysTickDeinit() {

	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

}

