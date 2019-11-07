#include <iostream>
#include <string>

#include "strender/flat_pieces.h"
#include "strender/piece.h"
#include "strender/strategy.h"

int main(int, char **) {
	using namespace strender;
	using namespace std::string_literals;

	//                   raw_[h]ats
	//        [h]eader <
	// root <            [n]ick < [r]aw_nick
	//        [m]essage - [raw]_message

	strategy msg_full("$h $m");

	strategy msg_header("<$h$n>", &msg_full);
	strategy msg_nick("$r", &msg_header);

	// Reverse raw message and surround with brackets.
	strategy msg_message([](const piece_map &pmap) -> std::string {
		std::string raw = pmap.at('r')->render();
		return "[" + std::string(raw.rbegin(), raw.rend()) + "]";
	}, &msg_full);

	// flat_pieces flats = {
	// 	{"h", nullptr},
	// 	{"hh", new piece("%")}
	// };
}
