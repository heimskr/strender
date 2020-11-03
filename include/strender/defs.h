#ifndef STRENDER_DEFS_H_
#define STRENDER_DEFS_H_

#include <functional>
#include <string>
#include <unordered_map>

namespace Strender {
	class StrNode;
	class Piece;

	using StringMap  = std::unordered_map<std::string, std::string>;
	using PieceMap   = std::unordered_map<std::string, Piece>;
	using StrNodeMap = std::unordered_map<std::string, StrNode *>;
	using SizeMap    = std::unordered_map<std::string, size_t>;
	using StrNode_f  = std::function<std::string(PieceMap &)>;
}

#endif
