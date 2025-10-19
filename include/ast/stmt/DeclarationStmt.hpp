#pragma once

#include "ast/stmt/Stmt.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    enum class DeclarationAccessibility
    {
        Public = 0,
        Private,
    };

    std::ostream &operator<<(std::ostream &os,
                             DeclarationAccessibility accessibility);

    struct DeclarationStmt : Stmt
    {
        Lexer::Token &identifier;
        DeclarationAccessibility accessibility =
            DeclarationAccessibility::Public;

        DeclarationStmt(Lexer::Token &identifier,
                        DeclarationAccessibility accessibility =
                            DeclarationAccessibility::Public,
                        bool is_definition = false);
        virtual ~DeclarationStmt() = default;

        bool is_definition() const;

        virtual void print(std::ostream &os, uint32_t tab) const override = 0;

      protected:
        bool is_definition_ = false;
    };

} // namespace AST
