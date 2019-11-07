#include "strender/piece.h"

namespace strender {
	bool piece::is_atomic() const {
		return children.empty();
	}
}
