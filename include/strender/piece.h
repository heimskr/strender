#ifndef STRENDER_PIECE_H_
#define STRENDER_PIECE_H_

#include <string>

#include "strender/piece_map.h"
#include "strender/strategy.h"

namespace strender {
	/**
	 * Represents an element in the hierarchy of a render tree.
	 */
	class piece {
		private:
			piece *parent;
			piece_map children {};

			std::string value {};
			bool cached;

			void delete_children();

		public:
			~piece();

			piece(const piece_map &children_, piece *parent_ = nullptr):
				parent(parent_), children(children_), cached(false) {}

			piece(const std::string &atomic_value, piece *parent_ = nullptr):
				parent(parent_), value(atomic_value), cached(true) {}

			piece & operator=(const std::string &);
			piece & operator=(const piece_map &);

			std::string render(const strategy &);
			std::string render();

			bool is_atomic() const;

			piece & operator+=(const std::pair<char, piece *> &);
			piece * operator[](char) const;
	};
}

#endif
