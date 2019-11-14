#include <iostream>
#include <list>
#include <sstream>

#include "strender/piece.h"
#include "strender/strnode.h"

#ifndef STRENDER_NO_ANSI
#include "lib/formicine/ansi.h"
#endif

#ifndef STRENDER_SIGIL
#define STRENDER_SIGIL "$"
#endif

namespace strender {
	// strnode::strnode(): empty(true), parent(nullptr), id("?") {}

	strnode::strnode(const char *id_, const std::string &format_, strnode *parent_):
		parent(parent_), format(format_), func({}), id(id_) { init(); }

	strnode::strnode(const char *id_, strnode_f func_, strnode *parent_):
		parent(parent_), format(""), func(func_),   id(id_) { init(); }

	strnode & strnode::operator=(const piece_map &map) {
		for (const auto &pair: map) {
			if (0 < input->count(pair.first))
				input->erase(pair.first);
			input->insert({pair.first, pair.second});
		}

		cached->clear();
		auto_assign();
		return *this;
	}

	std::string strnode::full_id() const {
		if (!id)
			return "NULL";
		return parent? parent->full_id() + ":" + id : id;
	}

	void strnode::init() {
		if (parent) {
			*parent += {id, this};
			input    = parent->input;
			cached   = parent->cached;
		} else {
			input  = std::make_shared<piece_map>();
			cached = std::make_shared<string_map>();
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

	std::string strnode::render(const piece_map &pieces) {
		*this = pieces;
		return render();
	}

	std::string strnode::render() {
		static const std::string sigil = STRENDER_SIGIL;

		if (is_cached())
			return get_cached();

		if (func)
			return cache(func(*input));

		// Find all positions and insert pairs of the identifiers and positions in order into a list.
		// The first position in the pair is a position within the unstripped format string.
		// The second is within the stripped format string.
		std::list<std::pair<std::string, std::pair<size_t, size_t>>> position_list {};
		const std::string stripped_format = ansi::strip(format);
		for (const auto &pair: *input) {
			// Look for $name$ in the format string.
			const std::string to_find = sigil + pair.first + sigil;
			const size_t pos = format.find(to_find);
			if (pos == std::string::npos)
				continue;

#ifdef STRENDER_NO_ANSI
			const std::pair<std::string, std::pair<size_t, size_t>> to_insert = {pair.first, {pos, 0}};
#else
			// If we found $name$ in the format string, find it in the stripped format string too.
			const size_t stripped_pos = stripped_format.find(to_find);
			const std::pair<std::string, std::pair<size_t, size_t>> to_insert = {pair.first, {pos, stripped_pos}};
#endif

			if (position_list.empty() || pos < position_list.front().second.first) {
				// If the list is empty or the new position is smaller than the front of the list, push it to the front.
				position_list.push_front(to_insert);
			} else {
				// Otherwise, look for the first element larger than the new position.
				bool inserted = false;
				for (auto it = position_list.begin(), end = position_list.end(); it != end; ++it) {
					if (pos < it->second.first) {
						// If one is found, insert the new position before it and stop.
						position_list.insert(it, to_insert);
						inserted = true;
						break;
					}
				}

				// If no larger value was found, the new position is the largest and is pushed to the back.
				if (!inserted)
					position_list.push_back(to_insert);
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
			const size_t unstripped_pos = pair.second.first;

			// Adjust the position (subtract the length of the $names$ and add the length
			// of all previously rendered text) and insert it into the positions map.
#ifdef STRENDER_NO_ANSI
			positions.insert({pair.first, unstripped_pos - name_lengths + rendered_lengths});
#else
			// If ANSI support is enabled, use the stripped position for the positions map.
			const size_t stripped_pos = pair.second.second;
			positions.insert({pair.first, stripped_pos - name_lengths + rendered_lengths});
#endif

			// Render the piece, then output the extra text from the format string followed by the rendered text.
			const std::string from_format = format.substr(last_pos, unstripped_pos - last_pos);
			const std::string rendered = pc.render();
#ifdef STRENDER_NO_ANSI
			oss << from_format << rendered;
#else
			oss << ansi::format(from_format + rendered);
#endif

			// The last position is right after the $name$.
			const size_t name_length = 2 + pair.first.length();
			last_pos = unstripped_pos + name_length;

			name_lengths += name_length;
#ifdef STRENDER_NO_ANSI
			rendered_lengths += rendered.length();
#else
			rendered_lengths += ansi::length(ansi::format(rendered));
#endif
		}

		// If this node has a parent, copy and adjust all of this node's positions.
		if (parent && 0 < parent->positions.count(id)) {
			const size_t offset = parent->positions.at(id);
			for (const auto &pair: positions)
				parent->positions.insert({pair.first, pair.second + offset});
		}

		if (last_pos != format.length()) {
#ifdef STRNODE_NO_ANSI
			oss << format.substr(last_pos);
#else
			oss << ansi::format(format.substr(last_pos));
#endif
		}

		return cache(oss.str());
	}

	void strnode::auto_assign() {
		if (parent && input)
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

	void strnode::copy(strnode *new_parent, strnode &out) const {
		out = *this;
		out.parent = new_parent;
		if (new_parent) {
			*new_parent += {id, &out};
			out.input = new_parent->input;
			out.cached = new_parent->cached;
		} else {
			out.input  = std::make_shared<piece_map>();
			out.cached = std::make_shared<string_map>();
		}
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

	strnode & strnode::operator+=(const std::pair<std::string, strnode *> &pair) {
		if (0 < children.count(pair.first))
			children.erase(pair.first);
		pair.second->parent = this;
		children.insert({pair.first, pair.second});
		return *this;
	}
}
