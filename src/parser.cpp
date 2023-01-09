#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "parser.hpp"


char Parser::peek() {
    if (this->it + 1 < this->end) {
        return *(this->it + 1);
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
    // TODO: store the read chars somewhere. For now we just iterate through
    char next = this->peek();
    if (next == 'h' || next == 'l') {
        this->state = CODE_END;
    } else {
        this->state = NUM;
    }
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
    this->state = DONE;
}

void Parser::update() {
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
	    default:
            this->codeEnd();
            break;
    }
}

// take start and end positions to the string to be parsed. Collect information about the encountered ANSI escape sequence to the given struct
void Parser::parseCode(struct AnsiCode &code, const std::string::const_iterator start, const std::string::const_iterator end) {
    if (*start != '\x1b')
	    return;

    this->state = ESC;
    this->it = std::string::const_iterator(start);
    this->end = end;

    this->result.fgColor = WHITE;
    this->result.bgColor = BLACK;
    this->result.style = REGULAR; 
    this->result.length = 0;
    
    while (this->it < this->end && this->state != DONE) {
        this->current = *(this->it);
        this->update();
	    ++this->result.length;
	    ++this->it;
    }

    code.fgColor = this->result.fgColor;
    code.bgColor = this->result.bgColor;
    code.style = this->result.style;
    code.length = this->result.length;
}
