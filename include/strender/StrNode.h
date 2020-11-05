#ifndef STRENDER_STRNODE_H_
#define STRENDER_STRNODE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "strender/Defs.h"
#include "strender/Piece.h"

#include "lib/formicine/ansi.h"

namespace Strender {
	/**
	 * There are two ways of producing a rendered string: format strings and custom functions. Format strings are
	 * simple strings that glue together subpieces, whereas custom functions are given all the subpieces and left to
	 * their own devices. A StrNode is pretty much an either-or container that contains both a format string and a
	 * custom function, but one is always blank.
	 */
	class StrNode {
		private:
			StrNode *parent = nullptr;
			std::string format;
			StrNode_f func;
			std::shared_ptr<PieceMap> input;
			std::shared_ptr<StringMap> cached;

			StrNodeMap children = {};

			const char *id;

			void init();

			bool isAtomic() const;
			bool isCached() const;
			const std::string & getCached() const;
			const std::string & cache(std::string &&);
			std::string fullID() const;

		public:
			/** Contains the positions within the *formatted* input string of each piece. */
			SizeMap positions;

			StrNode() = delete;
			StrNode(const char *, const std::string &, StrNode * = nullptr);
			StrNode(const char *, StrNode_f, StrNode * = nullptr);

			/** Feeds data to the input map. */
			StrNode & operator=(const PieceMap &);
			/** Overrides the StrNode with a function. */
			StrNode & operator=(StrNode_f);
			/** Overrides the StrNode with a format string. */
			StrNode & operator=(const std::string &);

			/** Consumes a map of input and renders a string. */
			std::string render(const PieceMap &);
			/** Renders a string. */
			std::string render();
			/** Adds this node and all its children to the input map. */
			void autoAssign();
			/** Clears the cache. */
			void resetAll();
			/** Removes this node and all its ancestors from the cache. */
			void uncache();
			/** Returns whether this node renders from a format string instead of a function. */
			bool isFormat() const;

			/** Creates a copy of this node with a different parent. */
			void copy(StrNode *new_parent, StrNode &out) const;

			/** Adds a child node, replacing any previous node that might've existed at the same key. */
			StrNode & operator+=(const std::pair<std::string, StrNode *> &);
	};
}

#endif
