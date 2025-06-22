#pragma once

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class IdentifierExpr : public Expr
    {
      private:
        Lexer::Token &identifier_;

      public:
        IdentifierExpr(Lexer::Token &identifier);
        ~IdentifierExpr() = default;

      public:
        Lexer::Token &identifier();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
