#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "parser.hpp"

char Parser::peek() {
    int ln = this->inp.length();
    if (this->index + 1 < ln) {
        return this->inp[this->index + 1];
    }
    return '\0';
}

void Parser::esc() {
    this->state = BRACK;
}

void Parser::brack() {
    char next = this->peek();
    if (next == '?') {
        this->type = PRIVATE_CODE;
        this->state = QUESTION_MARK;
    } else {
        this->type = COLOR_CODE;
        this->state = FIRST_NUM;
    }
}

void Parser::quest() {
    this->state = NUM;
}

void Parser::num() {
    // TODO: handle private codes like \x1b[?24.. 
}

void Parser::firstNum() {
    char next = this->peek();
    if (next == 'm') {
        this->state = CODE_END;
    } else if (next == ';') {
        this->state = SEMI;
        // TODO: handle single number codes 
    } else {
        if (this->current == '3') {
            this->state = FGCOLOR;
        } else {
            this->state = BGCOLOR; 
        }
    }
}

void Parser::foreground() {
    char next = this->peek();
    this->result.fgColor = COLOR(this->current - '0');
    if (next == ';')
        this->state = SEMI;
    else
        this->state = CODE_END;
}

void Parser::background() {
    char next = this->peek();
    this->result.bgColor = COLOR(this->current - '0');
    if (next == ';')
        this->state = SEMI;
    else
        this->state = CODE_END;

}

void Parser::semi() {
    this->state = FIRST_NUM;
}

void Parser::codeEnd() {
    this->state = TEXT;
}

void Parser::text() {
    if (this->current == '\x1b') {
        this->state = ESC;
    } else {
        this->result.text += this->current;
    }
}

void Parser::updateText() {
    switch (this->state) {
        case ESC:
            this->esc();
            break;
        case BRACK:
            this->brack();
            break;
        case QUESTION_MARK:
            this->quest();
            break;
        case NUM:
            this->num();
            break;
        case FIRST_NUM:
            this->firstNum();
            break;
        case FGCOLOR:
            this->foreground();
            break;
        case BGCOLOR:
            this->background();
            break;
        case SEMI:
            this->semi();
            break;
        case CODE_END:
            this->codeEnd();
            break;
        case TEXT:
            this->text();
            break;
    }
}

std::vector<std::string> splitBy(const std::string &s, const std::string &delim) {
    std::vector<std::string> split;
    size_t last = 0; 
    size_t next = 0;
    while ((next = s.find(delim, last)) != std::string::npos) {   
        split.push_back(s.substr(last, next-last)); 
        last = next + 1;
    }
    split.push_back(s.substr(last)); 
    return split;
}
   
// TODO parse the given string to a vector
std::vector<struct ParsedText> Parser::parse_to_words(const std::string &output) {
    std::vector<struct ParsedText> words;
    const std::vector<std::string> split = splitBy(output, " ");

    return words;
}

struct ParsedText Parser::parse(const std::string &word) {
    this->state = ESC;
    this->inp = word;
    this->result.fgColor = WHITE;
    this->result.bgColor = BLACK;
    this->result.style = REGULAR; 
    this->result.text = "";
    
    this->index = 0;
    for (char c : word) {
        this->current = c;
        this->updateText();
        this->index += 1;
    }

    struct ParsedText pt;
    pt.fgColor = this->result.fgColor;
    pt.bgColor = this->result.bgColor;
    pt.style = this->result.style;
    pt.text = this->result.text;

    return pt;
}
