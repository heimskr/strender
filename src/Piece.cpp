#include <iostream>

#include "strender/Piece.h"
#include "strender/StrNode.h"

namespace Strender {
	std::string Piece::render() {
		if (isAtomic())
			return str;
		return node->render();
	}
}
