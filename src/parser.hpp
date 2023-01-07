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
    DONE
} EVENT;

typedef enum {
    REGULAR = 0,
    BOLD,
    DIM,
    ITALIC,
    UNDERLINE,
    BLINKING
    // TODO: add the rest of the styles 
} TEXTSTYLE;


typedef enum {
    COLOR_CODE,
    PRIVATE_CODE
} CODE_TYPE;

struct AnsiCode {
    COLOR fgColor;
    COLOR bgColor;
    TEXTSTYLE style;
    unsigned length; // used to update iterator position
    AnsiCode() : fgColor{WHITE}, bgColor{BLACK}, style{REGULAR}, length{0} {
	//
    }
};

class Parser {
public: 
    void parseCode(struct AnsiCode &code, const std::string::const_iterator start, const std::string::const_iterator end);    
private:
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
    std::string::const_iterator it;
    std::string::const_iterator end; 
    struct AnsiCode result;
    char current;
    char peek();
    void update();
    EVENT state;
};

#endif
