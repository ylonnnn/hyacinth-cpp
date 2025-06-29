#pragma once

#include "ast/stmt/Stmt.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class DeclarationStmt : public Stmt
    {
      protected:
          Lexer::Token &name_;

      public:
        DeclarationStmt(Lexer::Token &name);
        virtual ~DeclarationStmt() = default;

        Lexer::Token &name() const;

        virtual void print(std::ostream &os, uint8_t tab) const override = 0;
    };

} // namespace AST
