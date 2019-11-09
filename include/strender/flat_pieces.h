#ifndef STRENDER_FLAT_PIECES_H_
#define STRENDER_FLAT_PIECES_H_

#include <cstring>
#include <set>

namespace strender {
	class piece;

	using flat_pair = std::pair<const char *, std::string>;

	struct flat_comp {
		bool operator()(const flat_pair &lpair, const flat_pair &rpair) const {
			const char *left = lpair.first, *right = rpair.first;
			if (std::strlen(left) == std::strlen(right))
				return std::strcmp(left, right) < 0;
			return std::strlen(left) < std::strlen(right);
		}
	};

	/**
	 * Represents a flattened tree of pieces.
	 * For example, a(b(d e) c(f g)) consists of a, ab, abd, abe, ac, acf and acg.
	 */
	using flat_pieces = std::set<flat_pair, flat_comp>;

	piece * expand(const flat_pieces &);
}

#endif
