#pragma once

#include "ast/expr/Expr.hpp"
#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    enum class UnaryType
    {
        Pre,
        Post
    };

    class UnaryExpr : public Expr
    {
      private:
        UnaryType unary_type_;

        Lexer::Token &operation_;
        std::unique_ptr<Expr> expr_;

      public:
        UnaryExpr(UnaryType unary_type, Lexer::Token &operation,
                  std::unique_ptr<Expr> expr);
        ~UnaryExpr() override = default;

      public:
        UnaryType unary_type();
        Lexer::Token &operation();
        Expr &expr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
