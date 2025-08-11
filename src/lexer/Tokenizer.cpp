#include <algorithm>
#include <cctype>
#include <cwctype>
#include <iostream>
#include <string_view>

#include "diagnostic/ErrorDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Tokenizer.hpp"

namespace Lexer
{
    Tokenizer::Tokenizer(Lexer &lexer)
        : lexer_(lexer), program_(lexer.program()), source_(program_.source())
    {
        initialize();
    }

    void Tokenizer::initialize()
    {
        reserved_.reserve(32);

        using namespace std::string_view_literals;
        using namespace TokenTypes;

        // Primary
        reserved_.insert_or_assign("true"sv, Primary::Boolean);
        reserved_.insert_or_assign("false"sv, Primary::Boolean);

        // Keywords
        reserved_.insert_or_assign("import"sv, Reserved::Import);
        reserved_.insert_or_assign("pub"sv, Reserved::Public);
        reserved_.insert_or_assign("priv"sv, Reserved::Private);
        reserved_.insert_or_assign("prot"sv, Reserved::Protected);

        // Types
        reserved_.insert_or_assign("struct"sv, Reserved::Struct);

        // Function-Related
        reserved_.insert_or_assign("fn"sv, Reserved::Function);
        reserved_.insert_or_assign("return"sv, Reserved::Return);

        // Variable-Related
        reserved_.insert_or_assign("let"sv, Reserved::Declaration);
    }

    bool Tokenizer::eof() const { return position_ >= source_.size(); }

    char Tokenizer::at(size_t pos) const
    {
        return pos >= source_.size() ? source_.back() : source_[pos];
    }

    char Tokenizer::current() const
    {
        if (position_ == 0)
            return source_.front();

        else if (eof())
            return source_.back();

        return source_[position_ - 1];
    }

    size_t Tokenizer::curr_pos() const
    {
        size_t s_size = source_.size();
        return position_ == 0        ? 0
               : position_ >= s_size ? s_size - 1
                                     : position_ - 1;
    }

    char Tokenizer::next()
    {
        return eof() ? source_.back() : source_[position_++];
    }

    char Tokenizer::peek()
    {
        return eof() ? source_.back() : source_[position_];
    }

    bool Tokenizer::match(char expect)
    {
        if (eof())
            return false;

        return source_[position_] == expect ? (next(), true) : false;
    }

    Token Tokenizer::create_token(const std::pair<size_t, size_t> &range,
                                  TokenType type)
    {
        auto &[start, end] = range;
        size_t len = (end - start) + 1;

        auto token =
            Token{source_.substr(start, len), program_.position_at(row_, col_),
                  program_.position_at(row_, col_ + (len - 1)), type};

        col_ += len;

        return token;
    }

    LexerResult Tokenizer::scan()
    {
        LexerResult result = {Core::ResultStatus::Success, {}, {}};
        result.lexer = &lexer_;

        result.data.reserve(
            std::max(static_cast<size_t>(128), source_.size() / 8));

        while (!eof())
        {
            Token token = scan(next());
            if (std::holds_alternative<TokenTypes::Miscellaneous>(token.type))
                continue;

            if (std::holds_alternative<TokenTypes::Invalid>(token.type))
            {
                result.error(Diagnostic::create_syntax_error(&token));
                continue;
            }

            result.data.push_back(std::move(token));
        }

        // EOF
        result.data.push_back(Token{"", program_.position_at(row_ + 1, 0),
                                    program_.position_at(row_ + 1, 0),
                                    TokenTypes::Miscellaneous::EndOfFile});

        return result;
    }

    Token Tokenizer::scan(char ch)
    {
        using namespace TokenTypes;

        size_t pos = curr_pos();

        switch (ch)
        {
            case '"':
            {
                do
                {
                    next();
                } while (!match('"'));

                return create_token({pos, curr_pos()}, Primary::String);
            }

            case '\'':
            {
                do
                {
                    next();
                } while (!match('\''));

                size_t c_pos = curr_pos();

                return (c_pos - 1) - pos > 1
                           ? create_token({pos, c_pos},
                                          Invalid::InvalidCharacter)
                           : create_token({pos, c_pos}, Primary::Character);
            }

            case ' ':
            case '\t':
            case '\r':
            case '\v':
            case '\f':
                return create_token({pos, pos}, Miscellaneous::Whitespace);

            case '\n':
            {
                Token token =
                    create_token({pos, pos}, Miscellaneous::LineBreak);
                row_++, col_ = 1;

                return token;
            }

            case '{':
                return create_token({pos, pos}, Delimeter::BraceOpen);

            case '}':
                return create_token({pos, pos}, Delimeter::BraceClose);

            case '[':
                return create_token({pos, pos}, Delimeter::BracketOpen);

            case ']':
                return create_token({pos, pos}, Delimeter::BracketClose);

            case '(':
                return create_token({pos, pos}, Delimeter::ParenthesisOpen);

            case ')':
                return create_token({pos, pos}, Delimeter::ParenthesisClose);

            case ',':
                return create_token({pos, pos}, Delimeter::Comma);

            case ';':
                return create_token({pos, pos}, Delimeter::Semicolon);

            case ':':
                return create_token({pos, pos}, Delimeter::Colon);

            case '.':
            {
                if (match('.'))
                    return create_token({pos, curr_pos()},
                                        Operator::Dot::Double);

                return create_token({pos, pos}, Operator::Dot::Single);
            }

            case '<':
            {
                // <-
                if (match('-'))
                    return create_token({pos, curr_pos()},
                                        Operator::Arrow::Left);

                // <=
                else if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Relational::LessThanEqual);

                // <
                return create_token({pos, curr_pos()},
                                    Operator::Relational::LessThan);
            }

            case '>':
            {
                // >=
                if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Relational::GreaterThanEqual);

                // >
                return create_token({pos, curr_pos()},
                                    Operator::Relational::GreaterThan);
            }

            case '=':
            {
                // ==
                if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Relational::Equal);

                // =
                return create_token({pos, curr_pos()},
                                    Operator::Assignment::Direct);
            }

            case '+':
            {
                // +=
                if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Assignment::Addition);

                // ++
                else if (match('+'))
                    return create_token({pos, curr_pos()},
                                        Operator::Unary::Increment);

                // +
                return create_token({pos, curr_pos()},
                                    Operator::ArithmeticUnary::Plus);
            }

            case '-':
            {
                if (std::isdigit(peek()))
                {
                    next();
                    return scan_numeric(pos);
                }

                // -=
                else if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Assignment::Subtraction);

                // --
                else if (match('-'))
                    return create_token({pos, curr_pos()},
                                        Operator::Unary::Decrement);

                // ->
                else if (match('>'))
                    return create_token({pos, curr_pos()},
                                        Operator::Arrow::Right);

                // -
                return create_token({pos, curr_pos()},
                                    Operator::ArithmeticUnary::Minus);
            }

            case '*':
            {
                // *=
                if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Assignment::Multiplication);
                // *
                return create_token({pos, curr_pos()},
                                    Operator::Arithmetic::Multiplication);
            }

            case '/':
            {
                // //
                if (match('/'))
                {
                    while (!match('\n'))
                        next();

                    Token token = create_token({pos, curr_pos()},
                                               Miscellaneous::CommentSingle);

                    row_++, col_ = 1;

                    return token;
                }

                // /=
                else if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Assignment::Division);
                // /
                return create_token({pos, curr_pos()},
                                    Operator::Arithmetic::Division);
            }

            case '%':
            {
                // %=
                if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Assignment::Modulo);

                // %
                return create_token({pos, curr_pos()},
                                    Operator::Arithmetic::Modulo);
            }

            case '!':
            {
                // !=
                if (match('='))
                    return create_token({pos, curr_pos()},
                                        Operator::Relational::NotEqual);
                // !
                return create_token({pos, curr_pos()}, Operator::Logical::Not);
            }

            case '&':
            {
                // && Logical AND
                if (match('&'))
                    return create_token({pos, curr_pos()},
                                        Operator::Logical::And);
                // & - Bitwise AND
                return create_token({pos, curr_pos()}, Operator::Bitwise::And);
            }

            case '|':
            {
                // ||
                if (match('|'))
                    return create_token({pos, curr_pos()},
                                        Operator::Logical::Or);
                // | - Bitwise OR
                return create_token({pos, curr_pos()}, Operator::Bitwise::Or);
            }

            case '^':
            {
                // ^^ - Possibly Exponent Operator

                // ^ - Bitwise XOR
                return create_token({pos, pos}, Operator::Bitwise::Xor);
            }

            default:
            {
                if (std::isalpha(ch))
                {
                    ch = peek();
                    while (std::isalnum(ch) || ch == '_')
                        next(), ch = peek();

                    size_t c_pos = curr_pos();
                    auto it =
                        reserved_.find(source_.substr(pos, (c_pos - pos) + 1));
                    if (it == reserved_.end())
                        return create_token({pos, c_pos}, Primary::Identifier);

                    return create_token({pos, c_pos}, it->second);
                }

                else if (std::isdigit(ch))
                    return scan_numeric(pos);

                return create_token({pos, pos}, Invalid::UnknownToken);
            }
        }
    }

    Token Tokenizer::scan_numeric(size_t start)
    {

        char ch = peek();
        size_t sc_start = position_ - 1;

        while (std::isdigit(ch) || ch == '_')
            next(), ch = peek();

        size_t c_pos = curr_pos();
        char front = source_[sc_start], back = source_[c_pos];

        if (!std::isdigit(front) || !std::isdigit(back))
            return create_token({start, c_pos},
                                TokenTypes::Invalid::UnknownToken);

        if (match('.'))
        {
            position_++;
            Token numeric = scan_numeric(start);

            numeric.type = TokenTypes::Primary::Float;
            return numeric;
        }

        return create_token({start, c_pos}, TokenTypes::Primary::Int);
    }

} // namespace Lexer
