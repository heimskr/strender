#ifndef STRENDER_STRNODE_H_
#define STRENDER_STRNODE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "strender/defs.h"
#include "strender/piece.h"

#include "lib/formicine/ansi.h"

namespace strender {
	/**
	 * There are two ways of producing a rendered string: format strings and custom functions. Format strings are
	 * simple strings that glue together subpieces, whereas custom functions are given all the subpieces and left to
	 * their own devices. A strnode is pretty much an either-or container that contains both a format string and a
	 * custom function, but one is always blank.
	 */
	class strnode {
		private:
			strnode *parent = nullptr;
			std::string format;
			strnode_f func;
			std::shared_ptr<piece_map> input;
			std::shared_ptr<string_map> cached;

			strnode_map children = {};

			const char *id;

			void init();

			bool is_atomic() const;
			bool is_cached() const;
			const std::string & get_cached() const;
			const std::string & cache(std::string &&);
			std::string full_id() const;

		public:
			/** Contains the positions within the *formatted* input string of each piece. */
			size_map positions;

			strnode() = delete;
			strnode(const char *, const std::string &, strnode * = nullptr);
			strnode(const char *, strnode_f, strnode * = nullptr);

			/** Feeds data to the input map. */
			strnode & operator=(const piece_map &);
			/** Overrides the strnode with a function. */
			strnode & operator=(strnode_f);
			/** Overrides the strnode with a format string. */
			strnode & operator=(const std::string &);

			/** Consumes a map of input and renders a string. */
			std::string render(const piece_map &);
			/** Renders a string. */
			std::string render();
			/** Adds this node and all its children to the input map. */
			void auto_assign();
			/** Clears the cache. */
			void reset_all();
			/** Removes this node and all its ancestors from the cache. */
			void uncache();
			/** Returns whether this node renders from a format string instead of a function. */
			bool is_format() const;

			/** Creates a copy of this node with a different parent. */
			void copy(strnode *new_parent, strnode &out) const;

			/** Adds a child node, replacing any previous node that might've existed at the same key. */
			strnode & operator+=(const std::pair<std::string, strnode *> &);
	};
}

#endif
