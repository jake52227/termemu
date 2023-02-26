#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <glad/glad.h>
#include "shader.hpp"


void Shader::set_text_color(const glm::vec3 &color)
{
    this->use();
    glUniform3f(glGetUniformLocation(this->id, "textColor"), color.x, color.y, color.z);
}

std::unique_ptr<Shader> Shader::create(const char *vertex_shader_path, const char *fragment_shader_path)
{
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(Shader());
    shader->compile(vertex_shader_path, fragment_shader_path);
    shader->use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Window::getWindowWidth()), 0.0f, static_cast<float>(Window::getWindowHeight()));
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    return shader;
}

std::string read_file_content(const char *path)
{
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void verify_compilation(unsigned int shader)
{
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "Shader: error with compilation:" << info_log << std::endl;
        exit(-1);
    }
}

unsigned comp_shader(std::string &source, unsigned type)
{
    unsigned id = glCreateShader(type);
    const char *cstr = source.c_str();
    glShaderSource(id, 1, (const char *const *)&cstr, NULL);
    glCompileShader(id);
    verify_compilation(id);
    return id;
}

unsigned make_program(unsigned &vertex, unsigned &fragment)
{
    unsigned int id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    int success;
    char info_log[512];

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, info_log);
        std::cerr << "Shader: error with linking:" << info_log << std::endl;
    }
    return id;
}

void Shader::compile(const char *vertex_path, const char *fragent_path)
{
    std::string vertex_source = read_file_content(vertex_path);
    std::string fragment_source = read_file_content(fragent_path);

    unsigned vertex = comp_shader(vertex_source, GL_VERTEX_SHADER);
    unsigned fragment = comp_shader(fragment_source, GL_FRAGMENT_SHADER);
    unsigned id = make_program(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    this->id = id;
}

void Shader::use()
{
    glUseProgram(this->id);
}

Shader::~Shader()
{
    glUseProgram(0);
    glDeleteProgram(this->id);
}
