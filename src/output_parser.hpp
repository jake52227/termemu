#ifndef OUTPUT_PARSER_HPP
#define OUTPUT_PARSER_HPP

#include <string>
#include <vector>

#define COLORSTART "\e["
#define COLOREND "\e[0m"

// esim: \e[0mSomeText\e[0m

enum class BackgroundColor {
    BLACK = 40,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    LIGHTGRAY,
    GRAY,
    LIGHTRED,
    LIGHTGREEN,
    LIGHTYELLOW,
    LIGHTBLUE,
    LIGHTMAGENTA,
    LIGHTCYAN,
    WHITE,
};

enum class ForegroundColor {
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    LIGHTGRAY,
    GRAY,
    LIGHTRED,
    LIGHTGREEN,
    LIGHTYELLOW,
    LIGHTBLUE,
    LIGHTMAGENTA,
    LIGHTCYAN,
    WHITE,
};

enum class DisplayStyle {
    NORMAL = 0,
    BOLD = 1,
    ITALIC = 3,
    UNDERLINE = 4,
    BLINK = 5,
    REVERSE = 7
};

struct Word {
    BackgroundColor bcolor;
    ForegroundColor fcolor;
    DisplayStyle style;
    std::string text;
};

std::vector<struct Word> parse_output(const std::string &out);

#endif
