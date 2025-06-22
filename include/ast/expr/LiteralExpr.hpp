#pragma once

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class LiteralExpr : public Expr
    {
      private:
        Lexer::Token &value_;

      public:
        LiteralExpr(Lexer::Token &token);
        ~LiteralExpr() override = default;

      public:
        Lexer::Token &value();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
