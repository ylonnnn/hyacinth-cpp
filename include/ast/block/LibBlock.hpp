#pragma once

#include "ast/block/GlobalBlock.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    class LibBlock : public GlobalBlock
    {
      private:
        Lexer::Token &name_;

      public:
        LibBlock(Lexer::Token &name);

        Lexer::Token &name();
        const Lexer::Token &name() const;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
