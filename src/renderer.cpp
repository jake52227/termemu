#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "renderer.hpp"
#include "window.hpp"
#include "macros.hpp"

void Renderer::clear_screen()
{
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clear_rectangle(float y, float x, float scale)
{
    // TODO: Starting from point (x,y), write transparent spaces to clear the line
}

void Renderer::load_chars(const char *font_path)
{
    FT_Library ft;
    FT_Face face;

    if (FT_Init_FreeType(&ft))
        errExit("Failed to initialize FreeType Library");

    std::string font_name = std::string(font_path);

    if (FT_New_Face(ft, font_name.c_str(), 0, &face))
        errExit("Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, PIXEL_SIZE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Failed to load Glyph for '" << c << "'" << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        this->symbols.insert(std::pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Renderer::configure_vao()
{
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &(this->vbo));
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

std::unique_ptr<Renderer> Renderer::create(Config &cfg)
{
    std::unique_ptr<Renderer> ren = std::make_unique<Renderer>(Renderer());
    std::string font_path = cfg.get_font_path();
    const char *fp = font_path.c_str();
    ren->load_chars(fp);
    ren->configure_vao();
    return ren;
}

Renderer::~Renderer()
{
    //
}

void Renderer::render_char(Character &ch, float x, float y, float scale)
{
    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},
        {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},

        {xpos, ypos + h, 0.0f, 0.0f},
        {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}
    };

    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::render_text(const std::string &text, float x, float y, float scale)
{
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->vao);

    for (const char &c : text)
    {
        Character &ch = symbols[c];
        render_char(ch, x, y, scale);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
