#pragma once

#include <unordered_map>

#include "lexer/Token.hpp"

namespace Lexer
{
    struct Lexer;
    struct LexerResult;

    Core::PositionRange range_to_curr(Lexer &lexer, size_t row, size_t col,
                                      size_t offset);

    struct Tokenizer
    {
        Lexer &lexer;
        Core::Program &program;

        std::string_view source;

        std::unordered_map<std::string_view, TokenType> reserved;

        size_t offset = 0, row = 1, col = 1;

        Tokenizer(Lexer &lexer);
        void initialize();

        bool bsof() const;
        bool eof() const;

        char at(size_t pos) const;

        size_t curr_offset() const;

        char next();
        char peek(size_t offset = 0) const;
        char current() const;

        bool match(char expect);
        void consume();

        void create_token(const std::pair<size_t, size_t> &range,
                          TokenType type);

        void ignore_whitespaces();

        void read_digit_seq(LexerResult &result, uint32_t base);
        void read_num(LexerResult &result);

        size_t read_char_seq(LexerResult &result, char terminator);
        void read_char(LexerResult &result);
        void read_str(LexerResult &result);

        void read_ident();

        LexerResult scan();
    };

} // namespace Lexer
