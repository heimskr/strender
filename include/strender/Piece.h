#ifndef STRENDER_PIECE_H_
#define STRENDER_PIECE_H_

#include <string>

#include "strender/Defs.h"

namespace Strender {
	/**
	 * Represents either a string or a StrNode.
	 */
	class Piece {
		private:
			std::string str {};
			StrNode *node = nullptr;

		public:
			Piece(const std::string &str_): str(str_) {}
			Piece(const char *str_): str(str_) {}
			Piece(StrNode *node_): node(node_) {}

			std::string render();
			inline bool isAtomic() const { return !node; }
	};
}

#endif