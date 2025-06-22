#pragma once

#include <functional>
#include <vector>

#include "lexer/Token.hpp"
#include "program/Program.hpp"

namespace Lexer
{
    using LexerTokenSkipPredicate = std::function<bool(Token &current)>;

    class Lexer
    {
      private:
        Program::ProgramFile &program_;
        std::vector<Token> tokens_;
        size_t position_ = 0;

      public:
        Lexer(Program::ProgramFile &program);

      protected:
      public:
        Program::ProgramFile &program();
        size_t position();

        void tokenize();
        bool eof();
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
