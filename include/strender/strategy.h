#ifndef STRENDER_STRATEGY_H_
#define STRENDER_STRATEGY_H_

#include <string>

#include "strender/piece_set.h"

namespace strender {
	using strategy_f = std::function<std::string(const piece_set &)>;
	/**
	 * There are two ways of producing a rendered string: format strings and custom functions. Format strings are a
	 * simple string that glues together subpieces, whereas custom functions are given all the subpieces and left to
	 * their own devices. A strategy is pretty much an either-or container that contains both a format string and a
	 * custom function, but one is always blank. 
	 */
	class strategy {
		private:
			bool empty;
			std::string format;
			strategy_f func;

		public:
			strategy();
			strategy(const std::string &);
			strategy(strategy_f);

			strategy & operator=(strategy_f);

			std::string apply(const piece_set &pset);

			operator bool() const;
	};
}

#endif
