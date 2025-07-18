#pragma once

#include <cstdint>

#include "ast/Node.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class Type : public Node
    {
      protected:
        Lexer::Token &value_;

      public:
        Type(Lexer::Token &value);
        virtual ~Type() = default;

      public:
        Lexer::Token &value();

        virtual std::string to_string() const = 0;
        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
