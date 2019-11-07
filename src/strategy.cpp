#include "strender/strategy.h"

namespace strender {
	strategy::strategy():
		empty(true), format(""), func({}) {}

	strategy::strategy(const std::string &format_):
		empty(false), format(format_), func({}) {}

	strategy::strategy(strategy_f func_):
		empty(false), format(""), func(func_) {}

	std::string strategy::apply(const piece_set &pset) {
		if (func)
			return func(pset);
		

		return "";
	}

	strategy & strategy::operator=(strategy_f func_) {
		func = func_;
		empty = false;
		format.clear();
		return *this;
	}

	strategy::operator bool() const {
		return empty;
	}
}
