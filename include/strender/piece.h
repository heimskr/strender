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
		public:
			piece *parent;
			piece_map children {};

			std::string value {};

			void delete_children();

		public:
			char id;
			bool cached;
			~piece();

			piece(char id_, piece *parent_ = nullptr, const piece_map &children_ = {}):
			parent(parent_), children(children_), id(id_), cached(false) {
				if (parent_)
					*parent_ += {id_, this};
			}

			piece(char id_, piece *parent_, const std::string &atomic_value):
			parent(parent_), value(atomic_value), id(id_), cached(true) {
				if (parent_)
					*parent_ += {id_, this};
			}

			piece(char id_, const piece_map &children_, piece &parent_):
			parent(&parent_), children(children_), id(id_), cached(false) {
				parent_ += {id_, this};
			}

			piece(char id_, const std::string &atomic_value, piece &parent_):
			parent(&parent_), value(atomic_value), id(id_), cached(true) {
				parent_ += {id_, this};
			}

			piece & operator=(const std::string &);
			piece & operator=(const piece_map &);

			std::string render(const strategy &);
			std::string render();

			bool is_atomic() const;

			piece & operator+=(const std::pair<char, piece *> &);
			piece & operator+=(const std::pair<char, piece &> &);
			piece * operator[](char) const;
	};
}

#endif
