#include "stm32f4xx.h"

namespace util {

	class Timer {
	public:
		typedef void (*TimerCb)(void *data);

		Timer();
		Timer(unsigned int time_ms, TimerCb cb);
		~Timer();

	private:
		TimerCb cb_;
		unsigned int time_ms_;

		static bool SysTickInit();
		static void SysTickDeinit();
	};

}
