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
			bool empty = false;
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

			strnode();
			strnode(const char *, const std::string &, strnode * = nullptr);
			strnode(const char *, strnode_f, strnode * = nullptr);
			~strnode();

			// strnode(const strnode &) = delete;
			// strnode & operator=(const strnode &) = delete;
			// strnode & operator=(strnode &&);

			strnode & operator=(const piece_map &);

			/** Overrides the strnode with a function. */
			strnode & operator=(strnode_f);
			/** Overrides the strnode with a format string. */
			strnode & operator=(const std::string &);

			std::string render(const piece_map &);
			std::string render();
			void auto_assign();
			void reset_all();
			void uncache();
			bool is_format() const;

			/** Creates a copy of this node with a different parent. */
			void copy(strnode *new_parent, strnode &out) const;

			strnode & operator+=(const std::pair<const char *, strnode *> &);
	};
}

#endif
