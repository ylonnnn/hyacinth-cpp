#pragma once

#include "ast/expr/Expr.hpp"
#include "ast/stmt/Stmt.hpp"

namespace AST
{
    class ExprStmt : public Stmt
    {
      protected:
        std::unique_ptr<AST::Expr> expr_;

      public:
        ExprStmt(std::unique_ptr<Expr> expr);
        ~ExprStmt() = default;

      public:
        AST::Expr &expr();

        std::unique_ptr<AST::Expr> &expr_ptr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
