#pragma once

#include <variant>

#include "ast/Node.hpp"
#include "ast/expr/Expr.hpp"
#include "ast/type/Type.hpp"
#include "core/value/Value.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    using IdentifierArgument =
        std::variant<std::unique_ptr<Type>, std::unique_ptr<Expr>>;

    struct Identifier : Node
    {
        Lexer::Token &identifier;
        std::vector<IdentifierArgument> arguments;

        Identifier(Lexer::Token &identifier,
                   std::vector<IdentifierArgument> &&arguments);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
