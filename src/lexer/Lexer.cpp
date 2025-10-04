#include <algorithm>
#include <cstddef>
#include <iostream>

#include "core/result/Result.hpp"
#include "lexer/Lexer.hpp"

namespace Lexer
{
    LexerResult::LexerResult(Core::ResultStatus status, void *data,
                             Diagnostic::DiagnosticList &&diagnostics)
        : Core::Result<void *>(status, data, std::move(diagnostics))
    {
    }

    Lexer::Lexer(Core::Program &program) : program(program), tokenizer(*this)
    {
        tokens.reserve(
            std::max(static_cast<size_t>(64), program.source.size() / 4));
    }

    Lexer::~Lexer() = default;
    // Lexer::~Lexer() { std::cout << "destroyed\n"; }

    LexerResult Lexer::tokenize() { return tokenizer.scan(); }

    bool Lexer::bsof() const { return position == 0; }

    bool Lexer::eof(bool absolute) const
    {
        return position >= (tokens.size() - !absolute);
    }

    Token *Lexer::at(size_t pos) { return eof() ? nullptr : &tokens[pos]; }

    Token *Lexer::next() { return eof() ? nullptr : &tokens[position++]; }

    Token *Lexer::peek() { return peekn(0); }

    Token *Lexer::peekn(size_t offset)
    {
        size_t pos = position + offset;
        return pos >= tokens.size() ? nullptr : &tokens[pos];
    }

    void Lexer::consume(size_t n)
    {
        size_t pos = position + n;
        if (pos <= tokens.size())
            position = pos;
    }

    Token &Lexer::current()
    {
        ssize_t max_idx = static_cast<ssize_t>(tokens.size() - 1);

        return tokens.at(std::clamp(static_cast<ssize_t>(position) - 1,
                                    static_cast<ssize_t>(0), max_idx));
    }

} // namespace Lexer
