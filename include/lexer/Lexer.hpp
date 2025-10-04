#pragma once

#include <vector>

#include "core/program/Program.hpp"
#include "core/result/Result.hpp"
#include "lexer/Token.hpp"
#include "lexer/Tokenizer.hpp"

namespace Lexer
{
    class Lexer;

    struct LexerResult : public Core::Result<void *>
    {
        LexerResult(Core::ResultStatus status, void *data,
                    Diagnostic::DiagnosticList &&diagnostics);
    };

    struct Lexer
    {
        Core::Program &program;
        std::vector<Token> tokens;
        size_t position = 0;

        Tokenizer tokenizer;

        Lexer(Core::Program &program);
        ~Lexer();

        LexerResult tokenize();

        bool bsof() const;
        bool eof(bool absolute = true) const;

        Token *at(size_t pos);
        Token *next();
        Token *peek();
        Token *peekn(size_t idx_pos);

        void consume(size_t n = 1);

        Token &current();
    };

} // namespace Lexer
