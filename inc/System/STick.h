#pragma once

#include <list>

#include "Utils/Timer.h"

namespace system {

class STick {
public:
	virtual ~STick();

	STick(const STick&) = delete;
	STick operator =(const STick&) = delete;

	static STick &Instance();

	std::list<utils::Timer *> *Callbacks();

	void RegisterCallback(utils::Timer *timer);
	void UnregisterCallback(utils::Timer *timer);

	unsigned Count();
	void operator++();
private:
	STick();

	volatile unsigned count_ = 0;
	std::list<utils::Timer *> callbacks_;
};


} // utils
