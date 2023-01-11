#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include "ansi_codes.hpp"
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
