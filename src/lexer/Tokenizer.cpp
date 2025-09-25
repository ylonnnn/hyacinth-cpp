#include <algorithm>
#include <cctype>
#include <cwctype>
#include <string_view>

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "lexer/Tokenizer.hpp"
#include "utils/dev.hpp"
#include "utils/numeric.hpp"

namespace Lexer
{
    Tokenizer::Tokenizer(Lexer &lexer)
        : lexer(lexer), program(lexer.program), source(program.source)
    {
        initialize();
    }

    void Tokenizer::initialize()
    {
        reserved.reserve(32);

        using namespace std::string_view_literals;

        // Primary
        reserved.insert_or_assign("true"sv, TokenType::Bool);
        reserved.insert_or_assign("false"sv, TokenType::Bool);

        // Keywords
        reserved.insert_or_assign("import"sv, TokenType::Import);
        // reserved.insert_or_assign("lib"sv, Reserved::Lib);

        reserved.insert_or_assign("pub"sv, TokenType::Pub);
        reserved.insert_or_assign("priv"sv, TokenType::Priv);
        reserved.insert_or_assign("prot"sv, TokenType::Prot);

        reserved.insert_or_assign("struct"sv, TokenType::Struct);

        reserved.insert_or_assign("fn"sv, TokenType::Fn);
        reserved.insert_or_assign("return"sv, TokenType::Return);

        reserved.insert_or_assign("let"sv, TokenType::Let);
        reserved.insert_or_assign("var"sv, TokenType::Var);
    }

    bool Tokenizer::eof() const { return offset >= source.size(); }

    char Tokenizer::at(size_t pos) const
    {
        return pos >= source.size() ? source.back() : source[pos];
    }

    char Tokenizer::current() const
    {
        if (offset == 0)
            return source.front();

        else if (eof())
            return source.back();

        return source[offset - 1];
    }

    size_t Tokenizer::curr_offset() const
    {
        return std::clamp(offset - 1, 0ul, source.size() - 1);
    }

    char Tokenizer::next() { return eof() ? '\0' : source[offset++]; }

    char Tokenizer::peek() const { return eof() ? '\0' : source[offset]; }

    char Tokenizer::peekn(size_t offset) const
    {
        size_t pos = this->offset + offset;
        return pos >= source.size() ? '\0' : source[offset];
    }

    bool Tokenizer::match(char expect)
    {
        if (eof())
            return false;

        return source[offset] == expect ? (next(), true) : false;
    }

    void Tokenizer::consume()
    {
        if (eof())
            return;

        offset++;
        p_col = col++;
    }

    void Tokenizer::create_token(const std::pair<size_t, size_t> &range,
                                 TokenType type, bool advance)
    {
        auto &[start, end] = range;
        size_t len = (end - start) + 1;

        lexer.tokens.emplace_back(
            source.substr(start, len),
            Core::PositionRange{program.position_at(row, col, start),
                                program.position_at(row, col + (len - 1), end)},
            type);

        if (advance)
            col += len;
    }

    void Tokenizer::ignore_whitespaces()
    {
        for (char c = peek(); !eof() && std::isspace(c); c = peek())
        {
            consume();

            if (c == '\n')
            {
                p_row = row++;
                p_col = col;
                col = 1;
            }
        }
    }

    void Tokenizer::read_digit_seq(uint32_t base)
    {
        bool allow_sep = false;
        auto pos = [&]() -> void {};

        for (char c = peek(); !eof(); c = peek())
        {
            if (std::isalnum(c))
            {
                if (!utils::is_digit_of(base, c))
                {
                    utils::todo("throw error: invalid numeric literal digit");
                    // diagnostic_t diagnostic = diagnostic_from(
                    //     DIAG_ERROR, INVALID_NUMLIT_DIGIT,
                    //     string_from("invalid numeric literal digit."),
                    //     (position_range_t){pos(lexer), pos(lexer)});

                    // result_error(&lexer->result, &diagnostic);
                }

                allow_sep = true;
                consume();

                continue;
            }

            else if (c == '_')
            {
                // If the next character is not a digit, a separator is not
                // allowed as it is the end of the digit sequence
                char n = peekn(1);
                if (n == '\0' || !std::isdigit(n))
                    allow_sep = false;

                if (!allow_sep)
                {
                    // diagnostic_t diagnostic = diagnostic_from(
                    //     DIAG_ERROR, UNEXPECTED_SEP,
                    //     string_from("unexpected numeric literal separator."),
                    //     (position_range_t){pos(lexer), pos(lexer)});

                    // result_error(&lexer->result, &diagnostic);
                }

                allow_sep = false;
                consume();

                continue;
            }

            return;
        }
    }

    void Tokenizer::read_num()
    {
        //
        char c = peek();
        uint32_t base = 10, s_row = row, s_col = col;
        size_t s_offset = offset;

        // Prefix-ed (0b, 0o, 0x)
        if (c == '0')
        {
            consume(), c = peek();
            base = c == 'b' ? 2 : c == 'o' ? 8 : c == 'x' ? 16 : UINT32_MAX;

            consume();
            if (eof())
            {
                utils::todo("throw error: malformed numeric literal");
                // diagnostic_t diagnostic = diagnostic_from(
                //     DIAG_ERROR, MALFORMED_LITERAL,
                //     string_from("malformed numeric literal."),
                //     POSRNG_SWTOCURR((*lexer), s_offset, s_row, s_col));

                // result_error(&lexer->result, &diagnostic);

                return;
            }
        }

        if (base == UINT32_MAX)
        {
            utils::todo("throw error: invalid numeric literal prefix");
            // diagnostic_t diagnostic = diagnostic_from(
            //     DIAG_ERROR, INVALID_NUMLIT_PREF,
            //     string_from("invalid numeric literal prefix"),
            //     POSRNG_SWTOCURR((*lexer), s_offset, s_row, s_col));

            // result_error(&lexer->result, &diagnostic);

            return;
        }

        read_digit_seq(base);

        if (match('.'))
        {
            read_digit_seq(base);
            create_token({s_offset, offset - 1}, TokenType::Float);

            return;
        }

        create_token({s_offset, offset - 1}, TokenType::Int);
    }

    size_t Tokenizer::read_char_seq(char terminator)
    {
        size_t len = 0, s_offset = offset - 1, s_row = p_row, s_col = p_col;

        for (char c = peek(); !match(terminator); c = peek())
        {
            // Unterminated Literal
            if (eof())
            {
                utils::todo("throw error: unterminated character sequence");
                // diagnostic_t diagnostic = diagnostic_from(
                //     DIAG_ERROR, UNTERMINATED_CHAR_SEQ,
                //     string_from("unterminated character sequence."),
                //     POSRNG_SWTOCURR((*lexer), s_offset, s_row, s_col));

                // result_error(&lexer->result, &diagnostic);

                return SIZE_MAX;
            }

            if (c == '\\')
                consume();

            consume();
            ++len;
        }

        return len;
    }

    void Tokenizer::read_char()
    {
        char quot_mark = peek();
        size_t s_offset = offset, s_row = row, s_col = col;

        consume();

        size_t seq_len = read_char_seq(quot_mark);
        if (seq_len == 1)
        {
            create_token({s_offset, offset - 1}, TokenType::Char);
            return;
        }

        utils::todo("throw error: invalid character literal");
        // diagnostic_t diagnostic =
        //     diagnostic_from(DIAG_ERROR, INVALID_LITERAL,
        //                     string_from("invalid character literal."),
        //                     POSRNG_SWTOCURR((*lexer), s_offset, s_row,
        //                     s_col));

        // result_error(&lexer->result, &diagnostic);
    }

    void Tokenizer::read_str()
    {
        char quot_mark = peek();
        size_t s_offset = offset;

        consume();

        if (read_char_seq(quot_mark) != SIZE_MAX)
            create_token({s_offset, offset - 1}, TokenType::String);
    }

    void Tokenizer::read_ident()
    {
        size_t s_offset = offset;

        for (char c = peek(); !eof() && (std::isalpha(c) || c == '_');
             c = peek())
            consume();

        std::string_view ident = source.substr(s_offset, offset - s_offset);

        auto it = reserved.find(ident);
        create_token({s_offset, offset - 1},
                     it == reserved.end() ? TokenType::Identifier : it->second);
    }

    LexerResult Tokenizer::scan()
    {
        LexerResult result{Core::ResultStatus::Success, nullptr, {}};

        while (!eof())
        {
            char c = peek();
            size_t ofs = offset;

            if (std::isspace(c))
            {
                ignore_whitespaces();
                ofs = offset, c = peek();
            }

            if (std::isdigit(c))
            {
                read_num();
                continue;
            }

            if (std::isalpha(c) || c == '_')
            {
                read_ident();
                continue;
            }

            if (c == '\'' || c == '"')
            {
                c == '\'' ? read_char() : read_str();
                continue;
            }

            consume();

            switch (c)
            {
                case '{':
                    create_token({ofs, offset - 1}, TokenType::LeftBrace);
                    continue;
                case '}':
                    create_token({ofs, offset - 1}, TokenType::RightBrace);
                    continue;
                case '[':
                    create_token({ofs, offset - 1}, TokenType::LeftBracket);
                    continue;
                case ']':
                    create_token({ofs, offset - 1}, TokenType::RightBracket);
                    continue;
                case '(':
                    create_token({ofs, offset - 1}, TokenType::LeftParen);
                    continue;
                case ')':
                    create_token({ofs, offset - 1}, TokenType::RightParen);
                    continue;
                case ',':
                    create_token({ofs, offset - 1}, TokenType::Comma);
                    continue;
                case ';':
                    create_token({ofs, offset - 1}, TokenType::Semicolon);
                    continue;

                case ':':
                {
                    if (match(':'))
                    {
                        create_token({ofs, offset - 1}, TokenType::DoubleColon);
                        continue;
                    }

                    create_token({ofs, offset - 1}, TokenType::Colon);
                    continue;
                }

                case '.':
                {
                    if (match('.'))
                    {
                        create_token({ofs, offset - 1}, TokenType::DoubleDot);
                        continue;
                    }

                    create_token({ofs, offset - 1}, TokenType::Dot);
                    continue;
                }

                case '<':
                {
                    // <-
                    if (match('-'))
                    {
                        char p = peek();
                        if (p == '\0')
                            break;

                        if (std::isdigit(p))
                        {
                            create_token({ofs, offset - 1}, TokenType::Less);
                            continue;
                        }

                        create_token({ofs, offset - 1}, TokenType::LessMinus);
                        continue;
                    }

                    // <=
                    else if (match('='))
                    {
                        create_token({ofs, offset - 1}, TokenType::LessEqual);
                        continue;
                    }

                    // <
                    create_token({ofs, offset - 1}, TokenType::Less);
                    continue;
                }

                case '>':
                {
                    // >=
                    if (match('='))
                    {
                        create_token({ofs, offset - 1},
                                     TokenType::GreaterEqual);
                        continue;
                    }

                    // >
                    create_token({ofs, offset - 1}, TokenType::Greater);
                    continue;
                }

                case '=':
                {
                    // ==
                    if (match('='))
                    {
                        create_token({ofs, offset - 1}, TokenType::EqualEqual);
                        continue;
                    }

                    // =
                    create_token({ofs, offset - 1}, TokenType::Equal);
                    continue;
                }

                case '+':
                {
                    // +=
                    if (match('='))
                    {
                        create_token({ofs, offset - 1}, TokenType::PlusEqual);
                        continue;
                    }
                    else if (match('+'))
                    {
                        create_token({ofs, offset - 1}, TokenType::PlusPlus);
                        continue;
                    }

                    // +
                    create_token({ofs, offset - 1}, TokenType::Plus);
                    continue;
                }

                case '-':
                {
                    char p = peek();
                    if (p == '\0')
                        break;

                    if (std::isdigit(p))
                    {
                        read_num();
                        continue;
                    }

                    // -=
                    else if (match('='))
                    {
                        create_token({ofs, offset - 1}, TokenType::MinusEqual);
                        continue;
                    }
                    else if (match('-'))
                    {
                        create_token({ofs, offset - 1}, TokenType::MinusMinus);
                        continue;
                    }
                    else if (match('>'))
                    {
                        create_token({ofs, offset - 1},
                                     TokenType::MinusGreater);
                        continue;
                    }

                    // -
                    create_token({ofs, offset - 1}, TokenType::Minus);
                    continue;
                }

                case '*':
                {
                    // *=
                    if (match('='))
                    {
                        create_token({ofs, offset - 1}, TokenType::StarEqual);
                        continue;
                    }

                    // *
                    create_token({ofs, offset - 1}, TokenType::Star);
                    continue;
                }

                case '/':
                {
                    // //
                    if (match('/'))
                    {
                        while (!match('\n'))
                            continue;

                        p_row = row++;
                        p_col = col;
                        col = 1;
                        continue;
                    }

                    // /=
                    else if (match('='))
                    {
                        create_token({ofs, offset - 1}, TokenType::SlashEqual);
                        continue;
                    }

                    // /
                    create_token({ofs, offset - 1}, TokenType::Slash);
                    continue;
                }

                case '%':
                {
                    // %=
                    if (match('='))
                    {
                        create_token({ofs, offset - 1},
                                     TokenType::PercentEqual);
                        continue;
                    }

                    // %
                    create_token({ofs, offset - 1}, TokenType::Percent);
                    continue;
                }

                case '!':
                {
                    // !=
                    if (match('='))
                    {
                        create_token({ofs, offset - 1}, TokenType::BangEqual);
                        continue;
                    }

                    // !
                    create_token({ofs, offset - 1}, TokenType::Bang);
                    continue;
                }

                case '&':
                {
                    // &&
                    if (match('&'))
                    {
                        create_token({ofs, offset - 1},
                                     TokenType::AmpersandAmpersand);
                        continue;
                    }

                    // &
                    create_token({ofs, offset - 1}, TokenType::Ampersand);
                    continue;
                }

                case '|':
                {
                    // ||
                    if (match('|'))
                    {
                        create_token({ofs, offset - 1}, TokenType::PipePipe);
                        continue;
                    }

                    // |
                    create_token({ofs, offset - 1}, TokenType::Pipe);
                    continue;
                }

                case '^':
                {
                    // ^^ (Exponent)
                    if (match('^'))
                    {
                        create_token({ofs, offset - 1}, TokenType::CaretCaret);
                        continue;
                    }

                    // ^
                    create_token({ofs, offset - 1}, TokenType::Caret);
                    continue;
                }

                default:
                {
                    if (eof())
                        break;

                    create_token({ofs, offset - 1}, TokenType::Illegal);
                }
            }
        }

        return result;
    }

} // namespace Lexer
