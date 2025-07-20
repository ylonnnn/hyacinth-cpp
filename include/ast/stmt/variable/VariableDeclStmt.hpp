#pragma once

#include "ast/common/Identifier.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/type/Type.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class VariableIdentifier : public Identifier
    {
      public:
        VariableIdentifier(Lexer::Token &name,
                           IdentifierMutabilityState mut_state,
                           std::unique_ptr<Type> type);
    };

    class VariableDeclarationStmt : public DeclarationStmt,
                                    public VariableIdentifier
    {
      protected:
        Lexer::Token &name_;

      public:
        VariableDeclarationStmt(Lexer::Token &name,
                                IdentifierMutabilityState mut_state,
                                std::unique_ptr<Type> type);

        virtual bool is_definition() const override;

        Lexer::Token &name();

        virtual void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
