#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "renderer.hpp"
#include "window.hpp"
#include "macros.hpp"

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

Renderer::Renderer(Config &cfg)
{
    std::string font_path = cfg.get_font_path();
    const char *fp = font_path.c_str();
    this->load_chars(fp);
    this->configure_vao();
}

Renderer::~Renderer()
{
    //
}

void Renderer::render_line(Shader &shader, Parser &parser, const std::string &text, DrawPos &drawPos)
{
    struct AnsiCode code;
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    auto it = text.cbegin();
    const auto end = text.cend();

    // Iterate the text. Find Ansi codes and extract them to the struct. Modify the rendering style based on this
    while (it < end)
    {
        if (*it == '\x1b')
        {
            parser.parseCode(code, it, end);
            it += code.length;
            color = get_color_as_vec(code.fgColor);
            // TODO update text style (bold, italic etc)
        }
        // find end position for this segment of text: either until next escape code or the end
        std::string::const_iterator pos(it);
        while (pos < end && *pos != '\x1b')
            ++pos;

        if (it < pos)
        {
            this->render_text(shader, it, pos, drawPos, 1, color);
            it = pos;
        }
        else
        {
            ++it;
        }
    }
}

void Renderer::render_text(Shader &shader, const std::string::const_iterator start, const std::string::const_iterator end, DrawPos &drawPos, float scale, glm::vec3 &color)
{
    shader.use();
    glUniform3f(glGetUniformLocation(shader.id, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    // TODO: buffer the renderings and do only a single rendering at the end

    // iterate through all characters
    for (auto c = start; c < end; ++c)
    {
        Character &ch = symbols[*c];

        float x = drawPos.getX();
        float y = drawPos.getY();
        drawPos.updatePos((ch.Advance >> 6) * scale);

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
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
