#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

typedef enum {
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    DEFAULT
} COLOR;


typedef enum {
    ESC,
    BRACK,
    QUESTION_MARK,
    NUM,
    FIRST_NUM,
    FGCOLOR,
    BGCOLOR,
    SEMI,
    CODE_END,
    TEXT,
} EVENT;

typedef enum {
    REGULAR = 0,
    BOLD,
    DIM,
    ITALIC,
    UNDERLINE,
    BLINKING
    // pari muutakin on, mutta näillä pärjää aluksi
} TEXTSTYLE;


typedef enum {
    COLOR_CODE,
    PRIVATE_CODE
} CODE_TYPE;

struct ParsedText {
    COLOR fgColor;
    COLOR bgColor;
    TEXTSTYLE style;
    std::string text;
};

class Parser {
public:
    std::vector<struct ParsedText> parse_to_words(const std::string &output);
private:
    struct ParsedText parse(const std::string &word);    
    void esc();
    void brack();
    void quest();
    void num();
    void firstNum();
    void foreground();
    void background();
    void semi();
    void codeEnd();
    void text();

    CODE_TYPE type;
    int index;
    std::string inp;
    struct ParsedText result;
    char current;
    char peek();
    void updateText();
    EVENT state;
};




#endif
