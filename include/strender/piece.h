#ifndef STRENDER_PIECE_H_
#define STRENDER_PIECE_H_

#include <string>

#include "strender/defs.h"

namespace strender {
	/**
	 * Represents either a string or a strnode.
	 */
	class piece {
		private:
			std::string str {};
			strnode *node = nullptr;

		public:
			piece(const std::string &str_): str(str_) {}
			piece(const char *str_): str(str_) {}
			piece(strnode *node_): node(node_) {}

			std::string render();
			inline bool is_atomic() const { return !node; }
	};
}

#endif