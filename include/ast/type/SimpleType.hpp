#pragma once

#include "ast/type/Type.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class SimpleType : public Type
    {
      private:
        Lexer::Token &identifier_;

      public:
        SimpleType(Lexer::Token &identifier);

        Lexer::Token &identifier();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
