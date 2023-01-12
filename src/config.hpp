#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config
{
public:
    Config();
    std::string get_font_path();

private:
    std::string font_path {};
};

#endif
