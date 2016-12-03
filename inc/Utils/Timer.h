#pragma once

#include <functional>

namespace utils {

struct Timer {

	Timer(std::function<void(void *)> cb, unsigned time, void *data, bool renew = true);
	~Timer();

	std::function<void(void *)> cb_;
	unsigned time_;
	unsigned callTime_;
	void *data_;
	bool renew_;

};

}
