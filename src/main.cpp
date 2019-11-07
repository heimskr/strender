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

	flat_pieces flats = {
		{"h", ""},
		{"hH", "%"},
		{"hn", ""},
		{"mr", "Hey there."},
		{"hnr", "FRESH"},
		{"m", ""},
	};

	piece *root = expand(flats);

	root->print();
	std::cout << "\n";

	const std::string rendered = root->render(s_full);
	std::cout << "\"" << rendered << "\"\n";
}
