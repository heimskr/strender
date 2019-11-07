#ifndef STRENDER_PIECE_MAP_H_
#define STRENDER_PIECE_MAP_H_

#include <unordered_map>

namespace strender {
	class piece;

	/**
	 * A piece map is a map of piece identifiers to strender pieces that are subpieces of a piece higher in the tree.
	 * They're passed to a strategy to render the higher piece.
	 */
	using piece_map = std::unordered_map<char, piece *>;
}

#endif
