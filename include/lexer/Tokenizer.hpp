#pragma once

#include "lexer/Token.hpp"
#include <unordered_map>

namespace Lexer
{
    class Lexer;
    struct LexerResult;

    class Tokenizer
    {
      private:
        Lexer &lexer_;
        Core::ProgramFile &program_;
        std::string_view source_;

        std::unordered_map<std::string_view, TokenType> reserved_;

        size_t position_ = 0;
        size_t row_ = 1, col_ = 1;

      public:
        Tokenizer(Lexer &lexer);

      protected:
        void initialize();

      public:
        bool eof() const;

        char at(size_t pos) const;

        char current() const;
        size_t curr_pos() const;

        char next();
        char peek();

        bool match(char expect);

        Token create_token(const std::pair<size_t, size_t> &range,
                           TokenType type, bool advance = true);

        LexerResult scan();
        Token scan(char ch);

        Token scan_numeric(size_t start);
    };

} // namespace Lexer
