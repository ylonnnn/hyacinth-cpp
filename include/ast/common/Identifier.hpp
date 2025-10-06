#pragma once

#include "ast/Node.hpp"
#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    using IdentifierArgument = std::variant<std::unique_ptr<Expr>>;

    struct Identifier : Node
    {
        Lexer::Token &identifier;
        std::vector<IdentifierArgument> arguments;

        Identifier(Lexer::Token &identifier,
                   std::vector<IdentifierArgument> &&arguments);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
