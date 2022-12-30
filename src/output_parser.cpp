#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <regex>
#include "output_parser.hpp"


std::vector<std::string> split_by(const std::string &out, char delim) {
    std::vector<std::string> split;
    std::stringstream ss(out);

    std::string s;
    while (getline(ss, s, delim))
	split.push_back(s);
    
    return split;
}

struct Word parse_to_word(std::string &s) {
    BackgroundColor bg = BackgroundColor::BLACK;
    ForegroundColor fg = ForegroundColor::BLACK;
    DisplayStyle dis = DisplayStyle::NORMAL;

    struct Word w;
    w.bcolor = bg;
    w.fcolor = fg;
    w.style = dis;
    w.text = "";

    // tämä matchaa oikein. Nice. pitää vielä parsia sen perusteella oikein. Nyt tämä vain crashaa
    std::string code_string("^.*[[0-9]*([0-9]*;)*([0-9]*)m.*$");

    std::string formatting_code;
    std::string text;
    
    if (std::regex_match(s, std::regex(code_string))) {
	size_t pos = s.find('[');
	size_t pos2 = s.find('m');

	if (pos == std::string::npos || pos2 == std::string::npos) {
	    formatting_code = s.substr(pos, pos2 - pos + 1);
	    text = s.substr(pos2 - pos + 2, std::string::npos);
	} else {
	    text = std::string(s);
	}

	// koodeja voi olla peräkkäin monta. Halutaan vain teksti talteen
	while (std::regex_match(text, std::regex(code_string))) {
	    if (pos == std::string::npos || pos2 == std::string::npos) {
		break;
	    }
	    text = s.substr(pos2 - pos + 2, std::string::npos);
	}

	
	//size_t pos = s.find('[');
	//size_t pos2 = s.find('m');
	//if (pos != std::string::npos && pos2 != std::string::npos) {
	//    formatting_code = s.substr(pos, pos2 - pos + 1);
	//    text = s.substr(pos2 - pos + 2, std::string::npos);
	//    std::cout << "formatting: " << formatting_code << std::endl;
	//    std::cout << "text:" << std::endl;
	//    for (char c : text) {
	//	std::cout << '\\' << c << std::endl;
	//    }
	//}
    } else {
	text = std::string(s);
    }
    
    w.text = text;
    return w;
}


// Parse escape codes for the soon to be rendered text
std::vector<struct Word> make_words(const std::vector<std::string> &split) {
    std::vector<struct Word> words;
    
    // TODO: pitäsi poimia merkkijonoista koodisto ja itse teksti Word-rakenteeseen
    for (std::string s : split) {
	struct Word w = parse_to_word(s);
	if (!w.text.empty())
	    words.push_back(w);
    }
    return words;
}


std::vector<struct Word> parse_output(const std::string &out) {
    const std::vector<std::string> split = split_by(out, ' ');
    const std::vector<struct Word> words = make_words(split);
    return words;
}
