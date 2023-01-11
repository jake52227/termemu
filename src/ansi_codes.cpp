#include "ansi_codes.hpp"

glm::vec3 get_color_as_vec(const COLOR color) {
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
    switch (color) {
        case BLACK:
            break;
        case RED:
            red = 1.0f;
            break;
        case GREEN:
            green = 1.0f;
            break;
        case YELLOW:
            red = 1.0f;
            green = 1.0f;
            break;
        case BLUE:
            blue = 1.0f;
            break;
        case MAGENTA:
            red = 1.0f;
            blue = 1.0f;
            break;
        case CYAN:
            green = 1.0f;
            blue = 1.0f;
            break;
        case WHITE:
        default:
            red = 1.0f;
            green = 1.0f;
            blue = 1.0f;
            break;
    }

    return glm::vec3(red, green, blue);
}
