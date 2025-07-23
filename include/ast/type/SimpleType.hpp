#pragma once

#include "ast/type/Type.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class SimpleType : public Type
    {
      public:
        SimpleType(Lexer::Token &identifier);

        Lexer::Token &identifier();

        std::string to_string() const override;
        void print(std::ostream &os, uint8_t tab) const override;

        bool operator==(const Type &other) const override;
    };

} // namespace AST
