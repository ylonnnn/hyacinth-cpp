#pragma once

#include <unordered_map>

#include "lexer/Token.hpp"

namespace Lexer
{
    struct Lexer;
    struct LexerResult;

    struct Tokenizer
    {
        Lexer &lexer;
        Core::ProgramFile &program;

        std::string_view source;

        std::unordered_map<std::string_view, TokenType> reserved;

        size_t offset = 0, row = 1, col = 1, p_row = 1, p_col = 1;

        Tokenizer(Lexer &lexer);
        void initialize();

        bool bsof() const;
        bool eof() const;

        char at(size_t pos) const;

        size_t curr_offset() const;

        char next();
        char peek() const;
        char peekn(size_t offset) const;
        char current() const;

        bool match(char expect);
        void consume();

        void create_token(const std::pair<size_t, size_t> &range,
                          TokenType type, bool advance = true);

        void ignore_whitespaces();

        void read_digit_seq(uint32_t base);
        void read_num();

        size_t read_char_seq(char terminator);
        void read_char();
        void read_str();

        void read_ident();

        LexerResult scan();
    };

} // namespace Lexer
