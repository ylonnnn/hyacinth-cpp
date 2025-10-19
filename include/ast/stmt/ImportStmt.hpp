#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    struct ImportStmt : DeclarationStmt
    {
        Lexer::Token &target;
        std::vector<Lexer::Token *> symbols;
        // TODO: Symbols

        ImportStmt(Lexer::Token &target, std::vector<Lexer::Token *> &&symbols);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
