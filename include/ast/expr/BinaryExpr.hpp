#pragma once

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class BinaryExpr : public Expr
    {
      private:
        Expr &left_;
        Lexer::Token &operation_;
        Expr &right_;

      public:
        BinaryExpr(Expr &left, Lexer::Token &operation, Expr &right);
        ~BinaryExpr() override = default;

      public:
        Expr &left();
        Lexer::Token &operation();
        Expr &right();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
