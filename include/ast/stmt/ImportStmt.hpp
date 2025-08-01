#pragma once

#include "ast/stmt/GlobalStmt.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class ImportStmt : public GlobalStmt
    {
      private:
        Lexer::Token &target_;

      public:
        ImportStmt(Lexer::Token &target);

        Lexer::Token &target();

        std::string_view target_strv() const;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
