#include <iostream>

#include "strender/flat_pieces.h"
#include "strender/piece.h"

namespace strender {
	piece * expand(const flat_pieces &pieces) {
		piece *root = new piece('*', nullptr);

		std::unordered_map<std::string, piece *> built {};

		for (const flat_pair &pair: pieces) {
			std::string name(pair.first);
			size_t nlen = name.length();
			if (nlen == 1) {
				if (pair.second.empty())
					built.insert({name, new piece(name.back(), root)});
				else
					built.insert({name, new piece(name.back(), root, pair.second)});
			} else {
				if (pair.second.empty())
					built.insert({name, new piece(name.back(), built.at(name.substr(0, nlen - 1)))});
				else
					built.insert({name, new piece(name.back(), built.at(name.substr(0, nlen - 1)), pair.second)});
			}

			std::cerr << "[" << pair.first << "]\n";
		}

		return root;
	}
}
