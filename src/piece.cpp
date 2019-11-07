#include "strender/piece.h"

namespace strender {
	piece::~piece() {
		delete_children();
	}

	void piece::delete_children() {
		for (auto &pair: children)
			delete pair.second;
		children.clear();
	}

	piece & piece::operator=(const std::string &str) {
		value = str;
		cached = true;
		delete_children();
		return *this;
	}

	piece & piece::operator=(const piece_map &map) {
		delete_children();
		children = map;
		value.clear();
		cached = false;
		return *this;
	}

	std::string piece::render(const strategy &strat) {
		if (cached || is_atomic())
			return value;
		cached = true;
		return value = strat.apply(children);
	}

	std::string piece::render() {
		if (!cached && !is_atomic())
			throw std::runtime_error("Render failed: piece isn't atomic or cached");
		return value;
	}

	bool piece::is_atomic() const {
		return children.empty();
	}

	piece & piece::operator+=(const std::pair<char, piece *> &pair) {
		if (children.count(pair.first) > 0)
			children.erase(pair.first);
		children.insert({pair.first, pair.second});
		return *this;
	}

	piece * piece::operator[](char ch) const {
		return children.at(ch);
	}
}
