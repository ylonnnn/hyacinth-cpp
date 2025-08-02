#pragma once

#include <vector>

#include "core/program/Program.hpp"
#include "core/result/Result.hpp"
#include "lexer/Token.hpp"
#include "lexer/Tokenizer.hpp"

namespace Lexer
{
    class Lexer;

    struct LexerResult : public Core::Result<std::vector<Token>>
    {
        Lexer *lexer = nullptr;

        LexerResult(Core::ResultStatus status, std::vector<Token> &&data,
                    Diagnostic::DiagnosticList diagnostics);
    };

    class Lexer
    {
      private:
        Core::ProgramFile &program_;
        std::vector<Token> tokens_;
        size_t position_ = 0;

        Tokenizer tokenizer_;

      public:
        Lexer(Core::ProgramFile &program);
        ~Lexer();

      protected:
      public:
        Core::ProgramFile &program();
        size_t position();
        size_t size();

        std::vector<Token> &tokens();

        LexerResult tokenize();
        bool eof(bool absolute = true);
        Token *at(size_t pos);

        void rewind(size_t pos = 0);
        void move(size_t pos);
        Token *next();
        Token *peek();
        Token *peekn(size_t idx_pos);
        Token &current();
    };

} // namespace Lexer
