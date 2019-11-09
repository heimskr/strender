#include <iostream>

#include "strender/piece.h"
#include "strender/strnode.h"

namespace strender {
	std::string piece::render() {
		if (is_atomic())
			return str;
		return node->render();
	}

	inline bool piece::is_atomic() const {
		return !node;
	}

/*
	piece & piece::operator=(const std::string &str) {
		value = str;
		cached = true;
		return *this;
	}

	std::string piece::render(const strnode &node) {
		if (cached || is_atomic())
			return value;
		cached = true;
		return value = node.apply(children);
	}

	std::string piece::render() {
		if (!cached && !is_atomic())
			throw std::runtime_error("Render failed: piece isn't atomic or cached");
		return value;
	}

	piece & piece::operator+=(const std::pair<char, piece *> &pair) {
		if (children.count(pair.first) > 0)
			children.erase(pair.first);
		pair.second->parent = this;
		children.insert({pair.first, pair.second});
		return *this;
	}

	piece & piece::operator+=(const std::pair<char, piece &> &pair) {
		return *this += {pair.first, &pair.second};
	}

	piece * piece::operator[](char ch) const {
		return children.at(ch);
	}
//*/
}
