#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "strender/defs.h"
#include "strender/strnode.h"

#include "lib/formicine/ansi.h"

int main(int, char **) {
	using namespace strender;
	using namespace std::string_literals;

	ansi::dbgstream << "-------------\n\e[2J\e[H";

	//                   raw_[h]ats
	//        [h]eader <
	// root <            [n]ick < [r]aw_nick
	//        [m]essage - [raw]_message

	strnode root("*", "$header$ $message$");

	strnode header("header", "<$hats$$nick$>", &root);
	strnode nick("nick", "$nick_raw$", &header);

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
	}, &root);

	root = {
		{"message_raw", "Hey there. This is a test message."},
		{"nick_raw", "Kai"},
		{"hats", "%"},
	};

	// std::cout << "\e[2m\"\e[0m" << root.render() << "\e[2m\"\e[0m\n";

	nick = [](piece_map &map) -> std::string {
		std::string raw = map.at("nick_raw").render();
		std::string out;
		std::transform(raw.begin(), raw.end(), std::back_inserter(out), [](char ch) {
			return ('a' <= ch && ch <= 'z')? ch - 'a' + 'A' : ch;
		});
		return out;
	};

	// std::cout << "\e[2m\"\e[0m" << root.render() << "\e[2m\"\e[0m\n";

	strnode root_("*", "^[cyan]hello $one$ $two$ $foo$ $three$ $four$ $five$^[/f] bye");
	strnode foo("foo", "[^b$bar$^B]", &root_);
	root_ = {
		{"one", "hi"}, {"two", "hello there"}, {"three", "what's ^uup^U"}, {"four", "idk :^^)"}, {"five", "k"},
		{"bar", "greetings"}
	};

	const std::string rendered = root_.render();
	std::cout << "\n\e[2m\"\e[0m" << rendered << "\e[2m\"\e[0m\n";
	std::cerr << "\nroot_.positions:\n";
	for (const auto &pair: root_.positions) {
		std::cerr << "{" << pair.first << ", " << pair.second << "}\n";
	}
}
