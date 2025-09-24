#pragma once

#include "ast/Node.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    struct PathSegment
    {
        Lexer::Token &identifier;
    };

    struct PathNode : virtual Node
    {
    };

} // namespace AST
