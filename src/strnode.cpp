#include <iostream>
#include <list>
#include <sstream>

#include "strender/piece.h"
#include "strender/strnode.h"

#ifndef STRENDER_SIGIL
#define STRENDER_SIGIL "$"
#endif

namespace strender {
	strnode::strnode(const char *id_, const std::string &format_, strnode *parent_):
		parent(parent_), format(format_), func({}), id(id_) { init(); }

	strnode::strnode(const char *id_, strnode_f func_, strnode *parent_):
		parent(parent_), format(""), func(func_),   id(id_) { init(); }
	
	strnode & strnode::operator=(piece_map &map) {
		input->swap(map);
		auto_assign();
		return *this;
	}

	strnode & strnode::operator=(const piece_map &map) {
		piece_map copy(map);
		input->swap(copy);
		auto_assign();
		return *this;
	}

	strnode::~strnode() {
		if (!parent) {
			delete input;
			delete cached;
		}
	}

	void strnode::init() {
		if (parent) {
			*parent += {id, this};
			input     = parent->input;
			cached    = parent->cached;
		} else {
			input     = new piece_map();
			cached    = new string_map();
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
		static const std::string sigil = STRENDER_SIGIL;

		if (is_cached())
			return get_cached();

		if (func)
			return cache(func(*input));

		// Find all positions and insert pairs of the identifiers and positions in order into a list.
		std::list<std::pair<const char *, size_t>> position_list {};
		for (const auto &pair: *input) {
			const size_t pos = format.find(sigil + pair.first + sigil);
			if (pos == std::string::npos)
				continue;
			std::cerr << "\"" << format << "\".find($" << pair.first << "$) == " << pos << "\n";

			if (position_list.empty() || pos < position_list.front().second) {
				// If the list is empty or the new position is smaller than the front of the list, push it to the front.
				position_list.push_front({pair.first, pos});
			} else {
				// Otherwise, look for the first element larger than the new position.
				bool inserted = false;
				for (auto it = position_list.begin(), end = position_list.end(); it != end; ++it) {
					if (pos < it->second) {
						// If one is found, insert the new position before it and stop.
						position_list.insert(it, {pair.first, pos});
						inserted = true;
						break;
					}
				}

				// If no larger value was found, the new position is the largest and is pushed to the back.
				if (!inserted)
					position_list.push_back({pair.first, pos});
			}
		}

		std::ostringstream oss;
		positions.clear();

		// Contains the index of the first character after the last sigil-identifier pair.
		size_t last_pos = 0;

		// In each iteration of the following for loop, this contains the sum of the length of all prior sigiled names.
		size_t name_lengths = 0;

		// Contains the sum of all prior renderings.
		size_t rendered_lengths = 0;

		for (const auto &pair: position_list) {
			piece &pc = input->at(pair.first);
			positions.insert({pair.first, pair.second - name_lengths + rendered_lengths});

			const std::string from_format = format.substr(last_pos, pair.second - last_pos);
			const std::string rendered = pc.render();
			oss << from_format << rendered;
			const size_t name_length = 2 + std::strlen(pair.first);
			last_pos = pair.second + name_length;

			name_lengths += name_length;
			rendered_lengths += rendered.length();
		}

		if (last_pos != format.length())
			oss << format.substr(last_pos);

		return cache(oss.str());
	}

	void strnode::auto_assign() {
		if (parent)
			input->insert({id, this});

		for (auto &pair: children)
			pair.second->auto_assign();
	}

	void strnode::reset_all() {
		cached->clear();
	}

	void strnode::uncache() {
		cached->erase(id);
		if (parent)
			parent->uncache();
	}

	bool strnode::is_format() const {
		return !func;
	}

	strnode & strnode::operator=(strnode_f func_) {
		func = func_;
		format.clear();
		uncache();
		return *this;
	}

	strnode & strnode::operator=(const std::string &format_) {
		format = format_;
		func = {};
		uncache();
		return *this;
	}

	strnode & strnode::operator+=(const std::pair<const char *, strnode *> &pair) {
		if (children.count(pair.first) > 0)
			children.erase(pair.first);
		pair.second->parent = this;
		children.insert({pair.first, pair.second});
		return *this;
	}
}
