#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include "output_parser.hpp"

std::vector<std::string> split_by_whitespace(const std::string &out) {
    std::vector<std::string> split;
    std::stringstream ss(out);

    std::string s;
    while (getline(ss, s, ' '))
	split.push_back(s);
    
    return split;
}

/* https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
   
\033[38;5;206m  ->  \033[38;5;<FG COLOR>m
\033[48;5;57m   ->  \033[48;5;<BG COLOR>m
\033[38;2;<r>;<g>;<b>m     #Select RGB foreground color
\033[48;2;<r>;<g>;<b>m     #Select RGB background color

*/
   
// Parse ANSI escape codes for the soon to be rendered text
std::vector<struct Word> make_words(const std::vector<std::string> &split) {
    std::vector<struct Word> words;

    BackgroundColor bg = BackgroundColor::BLACK;
    ForegroundColor fg = ForegroundColor::BLACK;
    DisplayStyle dis = DisplayStyle::NORMAL;

    // TODO: pitäsi poimia merkkijonoista koodisto ja itse teksti Word-rakenteeseen
    for (std::string s : split) {	
	struct Word w = (struct Word) {
	    bg,
	    fg,
	    dis,
	    std::string(s)
	};
	words.push_back(w);
    }
    return words;
}


std::vector<struct Word> parse_output(const std::string &out) {
    const std::vector<std::string> split = split_by_whitespace(out);
    const std::vector<struct Word> words = make_words(split);
    return words;
}
