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

    class DeclarationStmt : public GlobalNode, public Stmt
    {
      protected:
        Lexer::Token &name_;
        DeclarationAccessibility accessibility_ =
            DeclarationAccessibility::Private;

      public:
        DeclarationStmt(Lexer::Token &name);
        virtual ~DeclarationStmt() = default;

        Lexer::Token &name();
        DeclarationAccessibility accessibility() const;

        void set_accessibility(DeclarationAccessibility accessibility);

        virtual bool is_definition() const = 0;

        virtual void print(std::ostream &os, uint8_t tab) const override = 0;
    };

} // namespace AST
