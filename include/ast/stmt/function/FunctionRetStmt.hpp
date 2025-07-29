#pragma once

#include "ast/expr/Expr.hpp"
#include "ast/stmt/Stmt.hpp"

namespace AST
{
    class FunctionReturnStmt : public Stmt
    {
      private:
        std::unique_ptr<Expr> value_;

      public:
        FunctionReturnStmt(Core::Position position,
                           std::unique_ptr<Expr> value);

        void set_end_pos(size_t end_pos);

        Expr *value();

        std::unique_ptr<Expr> &value_ptr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
