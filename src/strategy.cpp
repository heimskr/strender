#include <iostream>
#include <list>
#include <sstream>

#include "strender/piece.h"
#include "strender/strategy.h"

namespace strender {
	strategy::strategy():
		empty(true), parent(nullptr), format(""), func({}), id('\0') {}

	strategy::strategy(char id_, const std::string &format_, strategy *parent_):
	empty(false), parent(parent_), format(format_), func({}), id(id_) {
		if (parent_)
			*parent_ += {id_, this};
	}

	strategy::strategy(char id_, strategy_f func_, strategy *parent_):
	empty(false), parent(parent_), format(""), func(func_), id(id_) {
		if (parent_)
			*parent += {id_, this};
	}

	std::string strategy::apply(const piece_map &pmap) const {
		static const std::string sigil = "$";

		if (empty) {
			if (pmap.size() != 1) {
				std::cerr << "[" << pmap.size() << "]\n";
				throw std::runtime_error("Expected pmap size of 1 for empty strategy");
			}
			return pmap.begin()->second->render();
		}

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
			std::cerr << "pc.id[" << pc->id << "] p.f[" << pair.first << "](" << children.size() << ") fmt[" << format << "]";
			for (const auto &pp: children)
				std::cerr << " " << pp.first;
			std::cerr << "\n";

			oss << format.substr(last_pos, pair.second - last_pos);
			if (pc->is_atomic()) {
				oss << pc->render();
			} else {
				std::cerr << "!atomic " << pc->id << ": " << pc->children.size() << "\n";
				if (children.count(pair.first) == 0) {
					oss << pc->render(strategy());
				} else {
					oss << pc->render(*children.at(pair.first));
				}
			}

			last_pos = pair.second + 2;
		}

		if (last_pos != format.length())
			oss << format.substr(last_pos);
		return oss.str();
	}

	strategy & strategy::operator=(strategy_f func_) {
		func = func_;
		empty = false;
		format.clear();
		return *this;
	}

	strategy & strategy::operator+=(const std::pair<char, strategy *> &pair) {
		if (children.count(pair.first) > 0)
			children.erase(pair.first);
		pair.second->parent = this;
		children.insert({pair.first, pair.second});
		std::cerr << "[s] insert to " << id << ": " << pair.first << " -> " << children.size() << "\n";
		return *this;
	}

	strategy::operator bool() const {
		return empty;
	}
}
