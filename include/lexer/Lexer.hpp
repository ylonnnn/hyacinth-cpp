#pragma once

#include <functional>
#include <vector>

#include "core/program/Program.hpp"
#include "lexer/Token.hpp"

namespace Lexer
{
    using LexerTokenSkipPredicate = std::function<bool(Token &current)>;

    class Lexer
    {
      private:
        Core::ProgramFile &program_;
        std::vector<Token> tokens_;
        size_t position_ = 0;

      public:
        Lexer(Core::ProgramFile &program);

      protected:
      public:
        Core::ProgramFile &program();
        size_t position();
        size_t size();

        void tokenize();
        bool eof(bool absolute = true);
        Token *at(size_t pos);

        void rewind(size_t pos = 0);
        void move(size_t pos);
        void skip(LexerTokenSkipPredicate predicate);
        Token *next();
        Token *peek();
        Token *peekn(size_t idx_pos);
        Token &current();
    };

} // namespace Lexer
