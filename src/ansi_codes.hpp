#ifndef ANSI_CODES_HPP
#define ANSI_CODES_HPP

#include <glm/glm.hpp>

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
    unsigned length; // the length of the escape sequence
    AnsiCode() : fgColor{WHITE}, bgColor{BLACK}, style{REGULAR}, length{0} {
	    //
    }
};

glm::vec3 get_color_as_vec(const COLOR color);

#endif