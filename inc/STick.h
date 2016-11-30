#pragma once

#include <list>

#include "Timer.h"

namespace util {

	class STick {
	public:
		virtual ~STick();

		STick(const STick&) = delete;
		STick operator =(const STick&) = delete;

		static STick &Instance();

		std::list<util::Timer *> *Callbacks();

		void RegisterCallback(util::Timer *timer);
		void UnregisterCallback(util::Timer *timer);

		unsigned Count();
		void operator++();
	private:
		STick();

		volatile unsigned count_ = 0;
		std::list<util::Timer *> callbacks_;
	};


} /* util */
