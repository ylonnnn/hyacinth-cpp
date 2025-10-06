#pragma once

#include "ast/GlobalNode.hpp"
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

    struct DeclarationStmt : GlobalNode, Stmt
    {
        Lexer::Token &name;
        DeclarationAccessibility accessibility =
            DeclarationAccessibility::Public;

        DeclarationStmt(Lexer::Token &name,
                        DeclarationAccessibility accessibility =
                            DeclarationAccessibility::Public,
                        bool is_definition = false);
        virtual ~DeclarationStmt() = default;

        bool is_definition() const;

        virtual void print(std::ostream &os, uint8_t tab) const override = 0;

      protected:
        bool is_definition_ = false;
    };

} // namespace AST
