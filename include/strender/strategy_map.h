#ifndef STRENDER_STRATEGY_MAP_H_
#define STRENDER_STRATEGY_MAP_H_

#include <unordered_map>

namespace strender {
	class strategy;

	using strategy_map = std::unordered_map<char, strategy *>;
}

#endif
