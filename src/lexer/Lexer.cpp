#include <algorithm>
#include <cstddef>
#include <iostream>
#include <re2/re2.h>
#include <string_view>
#include <variant>

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "program/Program.hpp"

namespace Lexer
{

    Lexer::Lexer(Program::ProgramFile &program) : program_(program)
    {
        tokens_.reserve(std::max((size_t)256, program.source().size() / 4));
    }

    Program::ProgramFile &Lexer::program() { return program_; }

    size_t Lexer::position() { return position_; }

    void Lexer::tokenize()
    {
        std::string_view source = program_.source();
        size_t row = 1, col = 1;

        for (size_t cursor = 0; cursor < source.size();)
        {
            re2::StringPiece source_piece(source.data() + cursor);

            for (auto &handler : TOKEN_HANDLERS)
            {
                const re2::RE2 &regex = handler.get_regex();
                re2::StringPiece matches[1];

                if (!regex.Match(source_piece, 0, source_piece.size(),
                                 re2::RE2::ANCHOR_START, matches, 1))
                    continue;

                auto &[full] = matches;

                const auto *misc =
                    std::get_if<TokenTypes::Miscellaneous>(&handler.type);

                if (misc == nullptr)
                    tokens_.push_back((Token){
                        full, program_.position_at(row, col), handler.type});

                size_t full_size = full.size();

                cursor += full_size;
                col += full_size;

                if (const auto *misc =
                        std::get_if<TokenTypes::Miscellaneous>(&handler.type))
                {
                    if (*misc == TokenTypes::Miscellaneous::LineBreak)

                    {
                        row++;
                        col = 1;
                    }
                }

                break;
            }
        }

        Token &last = tokens_.back();

        Program::Position position = last.position;
        position.col += last.value.size();

        tokens_.push_back((Token){" ", std::move(position),
                                  TokenTypes::Miscellaneous::EndOfFile});
    }

    bool Lexer::eof() { return position_ >= tokens_.size(); }

    Token *Lexer::at(size_t pos)
    {
        return pos < 0 || eof() ? nullptr : &tokens_[pos];
    }

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

    void Lexer::skip(LexerTokenSkipPredicate predicate)
    {
        while (!eof())
        {
            Token &current = *next();

            if (predicate(current))
                break;
        }
    }

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
        return tokens_.at(std::clamp(position_, (size_t)0, tokens_.size() - 1));
    }

} // namespace Lexer
