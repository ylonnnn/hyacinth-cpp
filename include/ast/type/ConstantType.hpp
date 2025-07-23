#pragma once

#include "ast/type/Type.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class ConstantType : public Type
    {
      private:
        Lexer::Token &constant_;

      public:
        ConstantType(Lexer::Token &constant);

        Lexer::Token &constant();

        std::string to_string() const override;
        void print(std::ostream &os, uint8_t tab) const override;

        bool operator==(const Type &other) const override;
    };

} // namespace AST
