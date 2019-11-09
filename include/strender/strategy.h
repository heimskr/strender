#ifndef STRENDER_STRATEGY_H_
#define STRENDER_STRATEGY_H_

#include <string>
#include <unordered_map>

#include "strender/piece_map.h"
#include "strender/strategy_map.h"

namespace strender {
	using strategy_f = std::function<std::string(const piece_map &)>;

	/**
	 * There are two ways of producing a rendered string: format strings and custom functions. Format strings are a
	 * simple string that glues together subpieces, whereas custom functions are given all the subpieces and left to
	 * their own devices. A strategy is pretty much an either-or container that contains both a format string and a
	 * custom function, but one is always blank. 
	 */
	class strategy {
		private:
			bool empty;
			strategy *parent;
			std::string format;
			strategy_f func;
			std::unordered_map<char, std::string> *data;

			strategy_map children {};
			char id;

			void init();

		public:
			strategy();
			strategy(char, const std::string &, strategy * = nullptr);
			strategy(char, strategy_f, strategy * = nullptr);

			~strategy();

			/** Overrides the strategy with a function. */
			strategy & operator=(strategy_f);

			std::string apply(const piece_map &pmap) const;

			strategy & operator+=(const std::pair<char, strategy *> &);
			operator bool() const;
	};
}

#endif
