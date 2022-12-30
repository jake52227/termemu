#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
public:
    ~Shader();
    unsigned id;
    void compile(const char *vertex_path, const char *fragent_path);
    void use();
};

#endif
