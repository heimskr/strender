
#ifndef STRENDER_PIECE_H_
#define STRENDER_PIECE_H_

#include <string>

#include "strender/defs.h"

namespace strender {
	class piece {
		private:
			std::string str {};
			strnode *node = nullptr;

		public:
			piece(const std::string &str_): str(str_) {}
			piece(const char *str_): str(str_) {}
			piece(strnode *node_): node(node_) {}

			std::string render();
			inline bool is_atomic() const;
	};



	/**
	 * Represents an element in the hierarchy of a render tree.
	 * /
	class piece {
		private:
			std::string value {};

		public:
			const char *id;
			bool cached;

			piece(const char *id_): id(id_), cached(false) {}
			piece(const char *id_, const std::string &atomic_value): value(atomic_value), id(id_), cached(true)  {}

			piece & operator=(const std::string &);

			std::string render(const strnode &);
			std::string render();

			void print();

			bool is_atomic() const;

			piece & operator+=(const std::pair<const char *, piece *> &);
			piece & operator+=(const std::pair<const char *, piece &> &);
			piece * operator[](const char *) const;
	}; //*/
}

#endif