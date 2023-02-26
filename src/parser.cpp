#include "parser.hpp"


std::unique_ptr<Parser> Parser::create()
{
    std::unique_ptr<Parser> parser = std::make_unique<Parser>(Parser());
    return parser;
}