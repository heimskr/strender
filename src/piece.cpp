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
}
