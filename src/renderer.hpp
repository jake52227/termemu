#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

/*
    The renderer class and things related to it are mostly copied from here: https://learnopengl.com/In-Practice/Text-Rendering
*/

#include <glad/glad.h>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <ft2build.h>
#include <vector>
#include <memory>
#include FT_FREETYPE_H

#include "shader.hpp"
#include "config.hpp"

struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Offset to advance to next glyph
};

class Renderer
{
public:
    static std::unique_ptr<Renderer> create(Config &cfg);
    ~Renderer();
    void render_text(const std::string &text, float x, float y, float scale);
    void clear_screen();
    void clear_rectangle(float y, float x, float scale);
private:
    unsigned vao;
    unsigned vbo;
    void load_chars(const char *font_path);
    void configure_vao();
    void render_char(Character &ch, float x, float y, float scale);
    std::unordered_map<GLchar, Character> symbols;
};

#endif
