#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "window.hpp"

class Shader
{
public:
    static std::unique_ptr<Shader> create(const char *vertex_shader_path, const char *fragment_shader_path);
    void set_text_color(const glm::vec3 &color);
    ~Shader();
    unsigned id;
    void compile(const char *vertex_path, const char *fragent_path);
    void use();
};

#endif
