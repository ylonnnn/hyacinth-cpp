#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/type/Type.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    enum class VariableMutabilityState : uint8_t
    {
        Mutable = 0,
        Immutable,
    };

    class VariableDeclarationStmt : public DeclarationStmt
    {
      protected:
        VariableMutabilityState mut_state_;
        std::unique_ptr<Type> type_;

      public:
        VariableDeclarationStmt(Lexer::Token &name,
                                VariableMutabilityState mut_state,
                                std::unique_ptr<Type> type);

        virtual bool is_definition() const override;

        VariableMutabilityState mut_state() const;
        Type &type();

        bool is_mutable() const;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
