#pragma once

#include "ast/stmt/DeclarationStmt.hpp"

namespace AST
{
    class StructDeclarationStmt : public DeclarationStmt
    {
      public:
        StructDeclarationStmt(Lexer::Token &name);

        virtual bool is_definition() const override;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
