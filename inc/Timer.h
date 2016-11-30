#pragma once

#include "stm32f4xx.h"

namespace util {

	struct Timer {
		typedef void (*TimerCb)(void *data);

		Timer(TimerCb cb, unsigned time, void *data, bool renew = true);
		~Timer();

		TimerCb cb_;
		unsigned time_;
		unsigned callTime_;
		void *data_;
		bool renew_;

	};

}
