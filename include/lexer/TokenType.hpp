#include <variant>

namespace Lexer
{
    namespace TokenTypes
    {
        enum class Reserved
        {
            Import,
            Lib,
            Public,
            Private,
            Protected,
            Struct,
            Class,
            Function,
            Return,
            Declaration,
            If,
            Else,
            For,
            While,
        };

        enum class Primary
        {
            Identifier,
            Int,
            Float,
            Boolean,
            Character,
            String,
        };

        namespace Operator
        {
            enum class Access
            {
                Dot,
                DoubleColon,
            };

            enum class Range
            {
                DoubleDot,
            };

            enum class Arrow
            {
                Left,
                Right,
            };

            enum class Assignment
            {
                Direct,
                Addition,
                Subtraction,
                Multiplication,
                Division,
                Modulo,
            };

            enum class Arithmetic
            {
                Plus,
                Minus,
                Multiplication,
                Division,
                Modulo,
            };

            enum class Unary
            {
                Increment,
                Decrement,
            };

            enum class Logical
            {
                Not,
                And,
                Or,
            };

            enum class Bitwise
            {
                Not,
                And,
                Or,
                Xor,
            };

            enum class Relational
            {
                Equal,
                NotEqual,
                LessThan,
                LessThanEqual,
                GreaterThan,
                GreaterThanEqual,
            };

        } // namespace Operator

        enum class Delimeter
        {
            ParenthesisOpen,
            ParenthesisClose,
            BraceOpen,
            BraceClose,
            BracketOpen,
            BracketClose,
            Comma,
            Semicolon,
            Colon,
        };

        enum class Miscellaneous
        {
            CommentSingle,
            CommentMultiple,
            LineBreak,
            Whitespace,
            EndOfFile,
        };

        enum class Invalid
        {
            UnknownToken,
            InvalidCharacter,
        };

        inline const char *to_string(TokenTypes::Reserved value)
        {
            switch (value)
            {
                case TokenTypes::Reserved::Import:
                    return "Reserved::Import";
                case TokenTypes::Reserved::Lib:
                    return "Reserved::Lib";
                case TokenTypes::Reserved::Public:
                    return "Reserved::Public";
                case TokenTypes::Reserved::Private:
                    return "Reserved::Private";
                case TokenTypes::Reserved::Protected:
                    return "Reserved::Protected";
                case TokenTypes::Reserved::Struct:
                    return "Reserved::Struct";
                case TokenTypes::Reserved::Class:
                    return "Reserved::Class";
                case TokenTypes::Reserved::Function:
                    return "Reserved::Function";
                case TokenTypes::Reserved::Return:
                    return "Reserved::Return";
                case TokenTypes::Reserved::Declaration:
                    return "Reserved::Declaration";
                case TokenTypes::Reserved::If:
                    return "Reserved::If";
                case TokenTypes::Reserved::Else:
                    return "Reserved::Else";
                case TokenTypes::Reserved::For:
                    return "Reserved::For";
                case TokenTypes::Reserved::While:
                    return "Reserved::While";
            }

            return "Reserved::Unknown";
        }

        inline const char *to_string(TokenTypes::Primary value)
        {
            switch (value)
            {
                case TokenTypes::Primary::Identifier:
                    return "Primary::Identifier";
                case TokenTypes::Primary::Int:
                    return "Primary::Int";
                case TokenTypes::Primary::Float:
                    return "Primary::Float";
                case TokenTypes::Primary::Boolean:
                    return "Primary::Boolean";
                case TokenTypes::Primary::Character:
                    return "Primary::Character";
                case TokenTypes::Primary::String:
                    return "Primary::String";
            }

            return "Primary::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Access value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Access::Dot:
                    return "Operator::Access::Dot";
                case TokenTypes::Operator::Access::DoubleColon:
                    return "Operator::Access::DoubleColon";
            }

            return "Operator::Dot::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Range value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Range::DoubleDot:
                    return "Operator::Range::DoubleDot";
            }

            return "Operator::Range::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Arrow value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Arrow::Left:
                    return "Operator::Arrow::Left";
                case TokenTypes::Operator::Arrow::Right:
                    return "Operator::Arrow::Right";
            }

            return "Operator::Arrow::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Assignment value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Assignment::Direct:
                    return "Operator::Assignment::Direct";
                case TokenTypes::Operator::Assignment::Addition:
                    return "Operator::Assignment::Addition";
                case TokenTypes::Operator::Assignment::Subtraction:
                    return "Operator::Assignment::Subtraction";
                case TokenTypes::Operator::Assignment::Multiplication:
                    return "Operator::Assignment::Multiplication";
                case TokenTypes::Operator::Assignment::Division:
                    return "Operator::Assignment::Division";
                case TokenTypes::Operator::Assignment::Modulo:
                    return "Operator::Assignment::Modulo";
            }

            return "Operator::Assignment::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Arithmetic value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Arithmetic::Plus:
                    return "Operator::Arithmetic::Plus";
                case TokenTypes::Operator::Arithmetic::Minus:
                    return "Operator::Arithmetic::Minus";
                case TokenTypes::Operator::Arithmetic::Multiplication:
                    return "Operator::Arithmetic::Multiplication";
                case TokenTypes::Operator::Arithmetic::Division:
                    return "Operator::Arithmetic::Division";
                case TokenTypes::Operator::Arithmetic::Modulo:
                    return "Operator::Arithmetic::Modulo";
            }

            return "Operator::Arithmetic::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Unary value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Unary::Increment:
                    return "Operator::Unary::Increment";
                case TokenTypes::Operator::Unary::Decrement:
                    return "Operator::Unary::Decrement";
            }

            return "Operator::Unary::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Logical value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Logical::Not:
                    return "Operator::Logical::Not";
                case TokenTypes::Operator::Logical::And:
                    return "Operator::Logical::And";
                case TokenTypes::Operator::Logical::Or:
                    return "Operator::Logical::Or";
            }

            return "Operator::Logical::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Bitwise value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Bitwise::Not:
                    return "Operator::Bitwise::Not";
                case TokenTypes::Operator::Bitwise::And:
                    return "Operator::Bitwise::And";
                case TokenTypes::Operator::Bitwise::Or:
                    return "Operator::Bitwise::Or";
                case TokenTypes::Operator::Bitwise::Xor:
                    return "Operator::Bitwise::Xor";
            }

            return "Operator::Bitwise::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Relational value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Relational::Equal:
                    return "Operator::Relational::Equal";
                case TokenTypes::Operator::Relational::NotEqual:
                    return "Operator::Relational::NotEqual";
                case TokenTypes::Operator::Relational::LessThan:
                    return "Operator::Relational::LessThan";
                case TokenTypes::Operator::Relational::LessThanEqual:
                    return "Operator::Relational::LessThanEqual";
                case TokenTypes::Operator::Relational::GreaterThan:
                    return "Operator::Relational::GreaterThan";
                case TokenTypes::Operator::Relational::GreaterThanEqual:
                    return "Operator::Relational::GreaterThanEqual";
            }

            return "Operator::Relational::Unknown";
        }

        inline const char *to_string(TokenTypes::Delimeter value)
        {
            switch (value)
            {
                case TokenTypes::Delimeter::ParenthesisOpen:
                    return "Delimeter::ParenthesisOpen";
                case TokenTypes::Delimeter::ParenthesisClose:
                    return "Delimeter::ParenthesisClose";
                case TokenTypes::Delimeter::BraceOpen:
                    return "Delimeter::BraceOpen";
                case TokenTypes::Delimeter::BraceClose:
                    return "Delimeter::BraceClose";
                case TokenTypes::Delimeter::BracketOpen:
                    return "Delimeter::BracketOpen";
                case TokenTypes::Delimeter::BracketClose:
                    return "Delimeter::BracketClose";
                case TokenTypes::Delimeter::Comma:
                    return "Delimeter::Comma";
                case TokenTypes::Delimeter::Semicolon:
                    return "Delimeter::Semicolon";
                case TokenTypes::Delimeter::Colon:
                    return "Delimeter::Colon";
            }

            return "Delimeter::Unknown";
        }

        inline const char *to_string(TokenTypes::Miscellaneous value)
        {
            switch (value)
            {
                case TokenTypes::Miscellaneous::CommentSingle:
                    return "CommentSingle";
                case TokenTypes::Miscellaneous::CommentMultiple:
                    return "CommentMultiple";
                case TokenTypes::Miscellaneous::LineBreak:
                    return "LineBreak";
                case TokenTypes::Miscellaneous::Whitespace:
                    return "Whitespace";
                case TokenTypes::Miscellaneous::EndOfFile:
                    return "EndOfFile";
            }

            return "Unknown";
        }

        inline const char *to_string(TokenTypes::Invalid value)
        {
            switch (value)
            {
                case TokenTypes::Invalid::UnknownToken:
                    return "UnknownToken";

                case TokenTypes::Invalid::InvalidCharacter:
                    return "InvalidCharacter";
            }

            return "Unknown";
        }

    } // namespace TokenTypes

    using TokenTypeOperator = std::variant<
        TokenTypes::Operator::Arrow, TokenTypes::Operator::Assignment,
        TokenTypes::Operator::Arithmetic, TokenTypes::Operator::Unary,
        TokenTypes::Operator::Logical, TokenTypes::Operator::Bitwise,
        TokenTypes::Operator::Relational>;

    using TokenType =
        std::variant<TokenTypes::Reserved, TokenTypes::Primary,
                     TokenTypes::Operator::Access, TokenTypes::Operator::Range,
                     TokenTypeOperator, TokenTypes::Delimeter,
                     TokenTypes::Miscellaneous, TokenTypes::Invalid>;

    bool operator==(TokenType &a, TokenType &b);

    inline const char *type_to_string(TokenType type)
    {
        return std::visit(
            [&](const auto &value)
            {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, TokenTypeOperator>)
                    return std::visit([&](const auto &val) -> const char *
                                      { return TokenTypes::to_string(val); },
                                      value);

                else
                    return TokenTypes::to_string(value);
            },
            type);
    }

} // namespace Lexer
