#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    struct ImportStmt : DeclarationStmt
    {
        Lexer::Token &target;
        // TODO: Symbols

        ImportStmt(Lexer::Token &target);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
