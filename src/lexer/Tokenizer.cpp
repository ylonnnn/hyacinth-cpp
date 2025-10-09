#include <algorithm>
#include <cctype>
#include <cwctype>
#include <string_view>

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "lexer/Tokenizer.hpp"
#include "utils/numeric.hpp"

namespace Lexer
{
    Core::PositionRange range_to_curr(Lexer &lexer, size_t row, size_t col,
                                      size_t offset)
    {
        Core::Program &program = lexer.program;
        Tokenizer tokenizer = lexer.tokenizer;

        Core::PositionRange range{
            program.position_at(row, col, offset),
            program.position_at(tokenizer.row, tokenizer.col - 1,
                                tokenizer.offset),
        };

        return range;
    }

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

        reserved.insert_or_assign("priv"sv, TokenType::Priv);
        reserved.insert_or_assign("prot"sv, TokenType::Prot);

        reserved.insert_or_assign("struct"sv, TokenType::Struct);

        reserved.insert_or_assign("fn"sv, TokenType::Fn);
        reserved.insert_or_assign("return"sv, TokenType::Return);

        reserved.insert_or_assign("var"sv, TokenType::Var);
        reserved.insert_or_assign("mut"sv, TokenType::Mut);
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
        return std::clamp(offset - 1, static_cast<size_t>(0ul),
                          source.size() - 1);
    }

    char Tokenizer::next()
    {
        if (eof())
            return '\0';

        if ((offset + 1) <= (source.size() - 1))
            ++col;

        return source[offset++];
    }

    char Tokenizer::peek(size_t offset) const
    {
        size_t pos = this->offset + offset;
        return pos >= source.size() ? '\0' : source[pos];
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

        if (++offset <= (source.size() - 1))
            ++col;
    }

    void Tokenizer::create_token(const std::pair<size_t, size_t> &range,
                                 TokenType type)
    {
        auto &[start, end] = range;
        size_t len = (end - start) + 1;

        lexer.tokens.emplace_back(
            source.substr(start, len),
            Core::PositionRange{program.position_at(row, col - len, start),
                                program.position_at(row, col - 1, end)},
            type);
    }

    void Tokenizer::ignore_whitespaces()
    {
        for (char c = peek(); !eof() && std::isspace(c); c = peek())
        {
            consume();

            if (c == '\n')
            {
                ++row;
                col = 1;
            }
        }
    }

    void Tokenizer::read_digit_seq(LexerResult &result, uint32_t base)
    {
        bool allow_sep = false;
        auto pos = [&]() -> Core::Position
        { return program.position_at(row, col, offset); };

        for (char c = peek(); !eof(); c = peek())
        {
            if (std::isalnum(c))
            {
                if (!utils::is_digit_of(base, c))
                {
                    result.error(
                        Core::PositionRange{pos(), pos()},
                        Diagnostic::ErrorType::InvalidNumericLiteralDigit,
                        "invalid numeric literal digit.");
                }

                allow_sep = true;
                consume();

                continue;
            }

            else if (c == '_')
            {
                // If the next character is not a digit, a separator is not
                // allowed as it is the end of the digit sequence
                char n = peek(1);
                if (n == '\0' || !std::isdigit(n))
                    allow_sep = false;

                if (!allow_sep)
                {
                    result.error(Core::PositionRange{pos(), pos()},
                                 Diagnostic::ErrorType::UnexpectedSeparator,
                                 "unexpected numeric literal separator.");
                }

                allow_sep = false;
                consume();

                continue;
            }

            return;
        }
    }

    void Tokenizer::read_num(LexerResult &result)
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
            if (eof() || !utils::is_digit_of(base, peek()))
            {
                result.error(range_to_curr(lexer, s_row, s_col, s_offset),
                             Diagnostic::ErrorType::MalformedLiteral,
                             "malformed numeric literal.");

                return;
            }
        }

        if (base == UINT32_MAX)
        {
            result.error(range_to_curr(lexer, s_row, s_col, s_offset),
                         Diagnostic::ErrorType::InvalidNumericLiteralPrefix,
                         "invalid numeric literal prefix.");

            return;
        }

        read_digit_seq(result, base);

        if (match('.'))
        {
            read_digit_seq(result, base);
            create_token({s_offset, offset - 1}, TokenType::Float);

            return;
        }

        create_token({s_offset, offset - 1}, TokenType::Int);
    }

    size_t Tokenizer::read_char_seq(LexerResult &result, char terminator)
    {
        size_t len = 0, s_offset = offset - 1, s_row = row, s_col = col;

        consume();

        for (char c = peek(); !match(terminator); c = peek())
        {
            // Unterminated Literal
            if (eof())
            {
                result.error(
                    range_to_curr(lexer, s_row, s_col, s_offset),
                    Diagnostic::ErrorType::UnterminatedCharacterSequence,
                    "unterminated character sequence.");

                return SIZE_MAX;
            }

            if (c == '\\')
                consume();

            consume();
            ++len;
        }

        return len;
    }

    void Tokenizer::read_char(LexerResult &result)
    {
        char quot_mark = peek();
        size_t s_offset = offset, s_row = row, s_col = col,
               seq_len = read_char_seq(result, quot_mark);

        if (seq_len == 1)
        {
            create_token({s_offset, offset - 1}, TokenType::Char);
            return;
        }

        result
            .error(range_to_curr(lexer, s_row, s_col, s_offset),
                   Diagnostic::ErrorType::InvalidLiteral,
                   "invalid character literal.")
            ->add_detail(std::make_unique<Diagnostic::Diagnostic>(
                Diagnostic::DiagnosticSeverity::Note,
                static_cast<uint32_t>(Diagnostic::NoteType::Suggestion),
                range_to_curr(lexer, s_row, s_col, s_offset),
                "use double quotes for strings."));
    }

    void Tokenizer::read_str(LexerResult &result)
    {
        char quot_mark = peek();
        size_t s_offset = offset;

        if (read_char_seq(result, quot_mark) != SIZE_MAX)
            create_token({s_offset, offset - 1}, TokenType::String);
    }

    void Tokenizer::read_ident()
    {
        size_t s_offset = offset;

        for (char c = peek(); !eof() && (std::isalnum(c) || c == '_');
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
                read_num(result);
                continue;
            }

            if (std::isalpha(c) || c == '_')
            {
                read_ident();
                continue;
            }

            if (c == '\'' || c == '"')
            {
                c == '\'' ? read_char(result) : read_str(result);
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
                        read_num(result);
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
                        {
                            consume();
                            continue;
                        }

                        ++row;
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
                    {
                        create_token({ofs, offset - 1}, TokenType::EndOfFile);
                        break;
                    }

                    create_token({ofs, offset - 1}, TokenType::Illegal);
                }
            }

            break;
        }

        return result;
    }

} // namespace Lexer
