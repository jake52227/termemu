#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "renderer.hpp"
#include "window.hpp"

#define check_success(key, msg) do { if (key) {std::cerr << msg << std::endl; exit(1);}  } while(0);

void Renderer::load_chars(const char *font_path) {
    FT_Library ft;
    FT_Face face;
  
    check_success(FT_Init_FreeType(&ft),
		  "Failed to initialize FreeType Library");

    std::string font_name = std::string(font_path);
    check_success(FT_New_Face(ft, font_name.c_str(), 0, &face),
		  "Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, PIXEL_SIZE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
	if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
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
	    face->glyph->bitmap.buffer
	    );

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

void Renderer::configure_vao() {
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

Renderer::Renderer(Config &cfg) {
    std::string font_path = cfg.get_font_path();
    const char *fp = font_path.c_str();
    this->load_chars(fp);
    this->configure_vao();
}

Renderer::~Renderer() {
    //
}

// go through the words vector, construct a color vector for each, render the text
void Renderer::render_words(Shader &shader, const std::vector<struct ParsedText> &words, float x, float y) {
    float orig_x = x;
    for (struct ParsedText w : words) {
	this->render_text(shader, w.text, orig_x, x, y, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

	x += PIXEL_SIZE;
	
	if (x >= getWindowWidth()) {
	    x = orig_x;
	    y -= PIXEL_SIZE;
	}
    }
}

void Renderer::render_user_text(Shader &shader, const char *text, float x, float y) {
    std::string s(text);
    this->render_text(shader, s, x, x, y, 1.0f, glm::vec3(0.0f, 0.5f, 0.5f));
}

void Renderer::render_text(Shader &shader, std::string &text, float x_start, float &x, float &y, float scale, glm::vec3 color) {
    shader.use();
    glUniform3f(glGetUniformLocation(shader.id, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    // TODO: puskuroi teksti ja tee yksi draw monen sijasta

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
	Character &ch = symbols[*c];

	if (*c == '\n') {
	    y -= 1.5 * PIXEL_SIZE;
	    x = x_start;
	}
      
	float xpos = x + ch.Bearing.x * scale;
	float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
      
	float w = ch.Size.x * scale;
	float h = ch.Size.y * scale;

	float vertices[6][4] = {
	    { xpos,     ypos + h,   0.0f, 0.0f },            
	    { xpos,     ypos,       0.0f, 1.0f },
	    { xpos + w, ypos,       1.0f, 1.0f },

	    { xpos,     ypos + h,   0.0f, 0.0f },
	    { xpos + w, ypos,       1.0f, 1.0f },
	    { xpos + w, ypos + h,   1.0f, 0.0f }           
	};
	
	glBindTexture(GL_TEXTURE_2D, ch.TextureID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	x += (ch.Advance >> 6) * scale; 
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
