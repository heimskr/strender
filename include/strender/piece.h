#ifndef STRENDER_PIECE_H_
#define STRENDER_PIECE_H_

#include <string>

#include "strender/piece_set.h"
#include "strender/strategy.h"

namespace strender {
	/**
	 * Represents an element in the hierarchy of a render tree.
	 */
	class piece {
		private:
			piece *parent;
			piece_set children {};

			std::string value {};

		public:
			piece(const piece_set &children_, piece *parent_ = nullptr):
				parent(parent_), children(children_) {}

			piece(const std::string &atomic_value, piece *parent_ = nullptr):
				parent(parent_), value(atomic_value) {}

			bool is_atomic() const;

	};
}

#endif
