#include <iostream>
#include <string>

#include "strender/defs.h"
#include "strender/strnode.h"

int main(int, char **) {
	using namespace strender;
	using namespace std::string_literals;

	//                   raw_[h]ats
	//        [h]eader <
	// root <            [n]ick < [r]aw_nick
	//        [m]essage - [raw]_message

	strnode s_full("*", "$header$ $message$");

	strnode s_header("header", "<$hats$$nick$>", &s_full);
	strnode s_nick("nick", "$nick_raw$", &s_header);

	// Reverse raw message and surround with brackets.
	strnode s_message("message", [](piece_map &map) -> std::string {
		std::string raw = map.at("message_raw").render();
		return "[" + std::string(raw.rbegin(), raw.rend()) + "]";
	}, &s_full);

	


	/*

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
	//*/
}
