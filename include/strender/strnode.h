#ifndef STRENDER_STRNODE_H_
#define STRENDER_STRNODE_H_

#include <string>
#include <unordered_map>

#include "strender/defs.h"
#include "strender/piece.h"

namespace strender {
	/**
	 * There are two ways of producing a rendered string: format strings and custom functions. Format strings are
	 * simple strings that glue together subpieces, whereas custom functions are given all the subpieces and left to
	 * their own devices. A strnode is pretty much an either-or container that contains both a format string and a
	 * custom function, but one is always blank.
	 */
	class strnode {
		private:
			strnode *parent;
			std::string format;
			strnode_f func;
			piece_map *input = nullptr;
			string_map *cached = nullptr;

			strnode_map children {};
			const char * id;

			void init();

			bool is_atomic() const;
			bool is_cached() const;
			const std::string & get_cached() const;
			const std::string & cache(std::string &&);

		public:
			strnode(const char *, const std::string &, strnode * = nullptr);
			strnode(const char *, strnode_f, strnode * = nullptr);
			strnode & operator=(piece_map &);
			strnode & operator=(const piece_map &);
			
			~strnode();

			/** Overrides the strnode with a function. */
			strnode & operator=(strnode_f);
			/** Overrides the strnode with a format string. */
			strnode & operator=(const std::string &);

			std::string render();
			void auto_assign();
			void reset_all();
			void uncache();
			bool is_format() const;

			strnode & operator+=(const std::pair<const char *, strnode *> &);
	};
}

#endif
