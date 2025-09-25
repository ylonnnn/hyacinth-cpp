#include <algorithm>
#include <cstddef>
#include <iostream>

#include "core/result/Result.hpp"
#include "lexer/Lexer.hpp"

namespace Lexer
{
    LexerResult::LexerResult(Core::ResultStatus status,
                             std::vector<Token> &&data,
                             Diagnostic::DiagnosticList diagnostics)
        : Core::Result<std::vector<Token>>(status, std::move(data),
                                           std::move(diagnostics))
    {
    }

    Lexer::Lexer(Core::ProgramFile &program)
        : program(program), tokenizer(*this)
    {
        tokens.reserve(
            std::max(static_cast<size_t>(64), program.source.size() / 4));
    }

    Lexer::~Lexer() = default;
    // Lexer::~Lexer() { std::cout << "destroyed\n"; }

    LexerResult Lexer::tokenize()
    {
        // LexerResult result = tokenizer_.scan();

        // tokens_ = std::move(result.data);

        // return result;
    }

    bool Lexer::bsof() const { return position == 0; }

    bool Lexer::eof(bool absolute) const
    {
        return position >= (tokens.size() - !absolute);
    }

    Token *Lexer::at(size_t pos) { return eof() ? nullptr : &tokens[pos]; }

    Token *Lexer::next() { return eof() ? nullptr : &tokens[position++]; }

    Token *Lexer::peek() { return peekn(position); }

    Token *Lexer::peekn(size_t offset)
    {
        size_t pos = position + offset;
        return pos >= (tokens.size() - 1) ? nullptr : &tokens[pos];
    }

    Token &Lexer::current()
    {
        auto max_idx = static_cast<ssize_t>(tokens.size() - 1);

        return tokens.at(
            std::clamp(static_cast<ssize_t>(position) - 1, 0l, max_idx));
    }

} // namespace Lexer
