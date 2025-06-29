#pragma once

#include "ast/expr/Expr.hpp"
#include "ast/stmt/VariableDeclStmt.hpp"

namespace AST
{
    class VariableDefinitionStmt : public VariableDeclarationStmt
    {
      private:
        std::unique_ptr<Expr> value_;

      public:
        VariableDefinitionStmt(Lexer::Token &name,
                               VariableMutabilityState mut_state,
                               std::unique_ptr<AST::Expr> value);

        bool is_definition() const override;

        Expr &value();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
