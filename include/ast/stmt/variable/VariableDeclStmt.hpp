#pragma once

// #include "ast/common/Identifier.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
// #include "ast/type/Type.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    // VariableIdentifier : Identifier
    // {
    //   public:
    //     VariableIdentifier(Lexer::Token &name,
    //                        IdentifierMutabilityState mut_state,
    //                        std::unique_ptr<Type> type);
    // };

    struct VariableDeclarationStmt : DeclarationStmt
    {
        Lexer::Token &name;

        // VariableDeclarationStmt(Lexer::Token &name,
        //                         IdentifierMutabilityState mut_state,
        //                         std::unique_ptr<Type> type);

        virtual void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
