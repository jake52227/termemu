#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include <memory>

class Parser
{
public:
    static std::unique_ptr<Parser> create();
private:
};

#endif
