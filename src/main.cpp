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

	strategy s_full('*', "$h $m");

	strategy s_header('h', "<$H$n>", &s_full);
	strategy s_nick('r', "$r", &s_header);

	// Reverse raw message and surround with brackets.
	strategy s_message('m', [](const piece_map &pmap) -> std::string {
		std::string raw = pmap.at('r')->render();
		return "[" + std::string(raw.rbegin(), raw.rend()) + "]";
	}, &s_full);

	// flat_pieces flats = {
	// 	{"h", nullptr},
	// 	{"hh", new piece("%")}
	// };

	piece *p_full = new piece('*', nullptr);
	piece *p_header  = new piece('h', p_full),  *p_hats = new piece('H', p_header);
	piece *p_message = new piece('m', p_full),  *p_message_raw = new piece('r', p_message, "Hey there.");
	piece *p_nick    = new piece('n', p_header), *p_nick_raw    = new piece('r', p_nick, "FRESH");

	const std::string rendered = p_full->render(s_full);
	std::cout << "\"" << rendered << "\"\n";
}
