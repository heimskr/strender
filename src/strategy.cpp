#include <list>
#include <sstream>

#include "strender/piece.h"
#include "strender/strategy.h"

namespace strender {
	strategy::strategy():
		empty(true), parent(nullptr), format(""), func({}) {}

	strategy::strategy(const std::string &format_, strategy *parent_):
		empty(false), parent(parent_), format(format_), func({}) {}

	strategy::strategy(strategy_f func_, strategy *parent_):
		empty(false), parent(parent_), format(""), func(func_) {}

	std::string strategy::apply(const piece_map &pmap) const {
		static const std::string sigil = "$";

		if (func)
			return func(pmap);

		// Find all positions and insert pairs of the identifiers and positions in order into a list.
		std::list<std::pair<char, size_t>> positions {};
		for (const auto &pair: pmap) {
			size_t pos = format.find(sigil + pair.first);
			if (positions.empty() || pos < positions.front().second) {
				// If the list is empty or the new position is smaller than the front of the list, push it to the front.
				positions.push_front({pair.first, pos});
			} else {
				// Otherwise, look for the first element larger than the new position.
				bool inserted = false;
				for (auto it = positions.begin(), end = positions.end(); it != end; ++it) {
					if (pos < it->second) {
						// If one is found, insert the new position before it and stop.
						positions.insert(it, {pair.first, pos});
						inserted = true;
						break;
					}
				}

				// If no larger value was found, the new position is the largest and is pushed to the back.
				if (!inserted)
					positions.push_back({pair.first, pos});
			}
		}

		std::ostringstream oss;
		size_t last_pos = 0; // Contains the index of the first character after the last sigil-identifier pair.
		for (const auto &pair: positions) {
			piece *pc = pmap.at(pair.first);
			oss << format.substr(last_pos, pair.second - last_pos)
			    << (pc->is_atomic()? pc->render() : pc->render(*children.at(pair.first)));
			last_pos = pair.second + 2;
		}

		return oss.str();
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
