#include <iostream>
#include <list>
#include <sstream>

#include "strender/piece.h"
#include "strender/strnode.h"

namespace strender {
	// strnode::strnode():
	// 	empty(true), parent(nullptr), format(""), func({}), id("") {}

	strnode::strnode(const char *id_, const std::string &format_, strnode *parent_):
	empty(false), parent(parent_), format(format_), func({}), id(id_) {
		init();
	}

	strnode::strnode(const char *id_, strnode_f func_, strnode *parent_):
	empty(false), parent(parent_), format(""), func(func_), id(id_) {
		init();
	}
	
	strnode & strnode::operator=(piece_map &map) {
		input->swap(map);
		return *this;
	}

	strnode & strnode::operator=(const piece_map &map) {
		piece_map copy(map);
		input->swap(copy);
		return *this;
	}

	strnode::~strnode() {
		if (parent == nullptr && !empty) {
			std::cerr << id << ": deleting data\n";
			delete input;
			delete cached;
		}
	}

	void strnode::init() {
		if (parent) {
			*parent += {id, this};
			input  = parent->input;
			cached = parent->cached;
		} else {
			input  = new std::unordered_map<const char *, piece>();
			cached = new string_map();
		}
	}

	bool strnode::is_atomic() const {
		return children.empty();
	}

	bool strnode::is_cached() const {
		return 0 < cached->count(id);
	}

	const std::string & strnode::get_cached() const {
		if (!is_cached())
			throw std::runtime_error("Attempted to get cached value when none exists");
		return cached->at(id);
	}

	const std::string & strnode::cache(std::string &&str) {
		cached->erase(id);
		cached->insert({id, std::move(str)});
		return cached->at(id);
	}

	std::string strnode::render() {
		static const std::string sigil = "$";

		if (is_cached())
			return get_cached();

		// if (empty) {
		// 	return cache(std::string(input->begin()->second));
		// }

		if (func)
			return cache(func(*input));

		// Find all positions and insert pairs of the identifiers and positions in order into a list.
		std::list<std::pair<const char *, size_t>> positions {};
		for (const auto &pair: *input) {
			size_t pos = format.find(sigil + pair.first + sigil);
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
			piece &pc = input->at(pair.first);

			oss << format.substr(last_pos, pair.second - last_pos);
			oss << pc.render();
			// if (pc->is_atomic()) {
			// } else {
			// 	if (children.count(pair.first) == 0) {
			// 		oss << pc->render(strnode());
			// 	} else {
			// 		oss << pc->render(*children.at(pair.first));
			// 	}
			// }

			last_pos = pair.second + 2 + std::strlen(pair.first);
		}

		if (last_pos != format.length())
			oss << format.substr(last_pos);

		return cache(oss.str());
	}

	strnode & strnode::operator=(strnode_f func_) {
		func = func_;
		empty = false;
		format.clear();
		return *this;
	}

	strnode & strnode::operator+=(const std::pair<const char *, strnode *> &pair) {
		if (children.count(pair.first) > 0)
			children.erase(pair.first);
		pair.second->parent = this;
		children.insert({pair.first, pair.second});
		return *this;
	}

	strnode::operator bool() const {
		return empty;
	}
}
