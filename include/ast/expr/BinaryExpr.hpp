#pragma once

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class BinaryExpr : public Expr
    {
      private:
        std::unique_ptr<Expr> left_;
        Lexer::Token &operation_;
        std::unique_ptr<Expr> right_;

      public:
        BinaryExpr(std::unique_ptr<Expr> left, Lexer::Token &operation,
                   std::unique_ptr<Expr> right);
        ~BinaryExpr() override = default;

      public:
        Expr &left();
        Lexer::Token &operation();
        Expr &right();

        std::unique_ptr<Expr> &left_ptr();
        std::unique_ptr<Expr> &right_ptr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
