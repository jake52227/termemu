#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include <glad/glad.h>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.hpp"
#include "output_parser.hpp"
#include "config.hpp"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class Renderer {
public:
    Renderer(Config &cfg);
    ~Renderer();
    void render_words(Shader &shader, const std::vector<struct Word> &words, float x, float y);
    void render_user_text(Shader &shader, const char *text, float x, float y);
private:
    void render_text(Shader &shader, std::string &text, float x_start, float &x, float &y, float scale, glm::vec3 color);
    unsigned vao;
    unsigned vbo;
    void load_chars(const char *font_path);
    void configure_vao();
    std::unordered_map<GLchar, Character> symbols;
};

#endif
