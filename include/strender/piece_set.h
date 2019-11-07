#ifndef STRENDER_PIECE_SET_H_
#define STRENDER_PIECE_SET_H_

#include <unordered_map>

namespace strender {
	class piece;

	/**
	 * A piece set is a set of strender pieces that are subpieces of a piece higher in the tree.
	 * They're passed to a strategy to render the higher piece.
	 * Piece sets map piece identifiers to pieces.
	 */
	using piece_set = std::unordered_map<char, piece *>;
}

#endif
