#pragma once

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    struct LiteralExpr : Expr
    {
        Lexer::Token &value;

        LiteralExpr(Lexer::Token &token);
        ~LiteralExpr() override = default;

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
