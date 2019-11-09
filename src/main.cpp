#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
		static std::string colors[] = {"1", "8;5;202", "3", "2", "6", "4", "8;5;56", "5"};
		std::ostringstream oss;
		int i = -1;
		for (char ch: map.at("message_raw").render()) {
			if (std::iswspace(ch))
				oss << ch;
			else
				oss << "\e[3" << colors[++i % (sizeof(colors) / sizeof(std::string))] << "m" << ch;
		}
		oss << "\e[0m";
		return oss.str();
	}, &s_full);

	s_full = {
		{"message_raw", "Hey there. This is a test message."},
		{"nick_raw", "FRESH"},
		{"hats", "%"},
	};

	std::string rendered = s_full.render();
	std::cout << "\e[2m\"\e[0m" << rendered << "\e[2m\"\e[0m\n";
}
