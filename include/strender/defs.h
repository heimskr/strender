#ifndef STRENDER_DEFS_H_
#define STRENDER_DEFS_H_

#include <string>
#include <unordered_map>

namespace strender {
	class strnode;
	class piece;

	using string_map  = std::unordered_map<std::string, std::string>;
	using piece_map   = std::unordered_map<std::string, piece>;
	using strnode_map = std::unordered_map<std::string, strnode *>;
	using size_map    = std::unordered_map<std::string, size_t>;
	using strnode_f   = std::function<std::string(piece_map &)>;
}

#endif
