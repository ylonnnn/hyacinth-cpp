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
        : program_(program), tokenizer_(*this)
    {
        tokens_.reserve(
            std::max(static_cast<size_t>(256), program.source().size() / 4));
    }

    Core::ProgramFile &Lexer::program() { return program_; }

    size_t Lexer::position() { return position_; }

    size_t Lexer::size() { return tokens_.size(); }

    LexerResult Lexer::tokenize()
    {
        LexerResult result = tokenizer_.scan();

        tokens_ = std::move(result.data);

        return result;
    }

    bool Lexer::eof(bool absolute)
    {
        return position_ >= (tokens_.size() - !absolute);
    }

    Token *Lexer::at(size_t pos) { return eof() ? nullptr : &tokens_[pos]; }

    void Lexer::rewind(size_t pos)
    {
        if (pos > position_)
        {
            std::cerr << "Cannot rewind to a position that is still yet to be "
                         "reached!\n";
            return;
        }

        position_ = pos;
    }

    void Lexer::move(size_t pos) { position_ = pos; }

    Token *Lexer::next()
    {
        Token *token = peek();
        if (token == nullptr)
            return nullptr;

        position_++;

        return token;
    }

    Token *Lexer::peek() { return eof() ? nullptr : peekn(position_); }

    Token *Lexer::peekn(size_t idx_pos)
    {
        return idx_pos >= tokens_.size() ? nullptr : &tokens_.at(idx_pos);
    }

    Token &Lexer::current()
    {
        using signed_size_t = std::make_signed_t<size_t>;
        signed_size_t max_idx = static_cast<signed_size_t>(tokens_.size() - 1);

        return tokens_.at(std::clamp(static_cast<signed_size_t>(position_) - 1,
                                     static_cast<signed_size_t>(0), max_idx));
    }

} // namespace Lexer
