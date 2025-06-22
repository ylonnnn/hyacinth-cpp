#include <variant>

namespace Lexer
{
    namespace TokenTypes
    {
        enum class Reserved
        {
            Import,
            Public,
            Private,
            Protected,
            Function,
            Return,
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
            InvalidCharacter,
            String,
        };

        namespace Operator
        {

            enum class Dot
            {
                Single,
                Double,
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

            enum class ArithmeticUnary
            {
                Plus,
                Minus,
            };

            enum class Arithmetic
            {
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

            enum class Relational
            {
                Equal,
                NotEqual,
                LessThan,
                LessThanEqual,
                GreaterThan,
                GreatherThanEqual,
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

        inline const char *to_string(TokenTypes::Reserved value)
        {
            switch (value)
            { case TokenTypes::Reserved::Import:
                    return "Reserved::Import";
                case TokenTypes::Reserved::Public:
                    return "Reserved::Public";
                case TokenTypes::Reserved::Private:
                    return "Reserved::Private";
                case TokenTypes::Reserved::Protected:
                    return "Reserved::Protected";
                case TokenTypes::Reserved::Function:
                    return "Reserved::Function";
                case TokenTypes::Reserved::Return:
                    return "Reserved::Return";
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
                case TokenTypes::Primary::InvalidCharacter:
                    return "Primary::InvalidCharacter";
                case TokenTypes::Primary::String:
                    return "Primary::String";
            }

            return "Primary::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Dot value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Dot::Single:
                    return "Operator::Dot::Single";
                case TokenTypes::Operator::Dot::Double:
                    return "Operator::Dot::Double";
            }

            return "Operator::Dot::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Arrow value)
        {
            switch (value)
            {
                case TokenTypes::Operator::Arrow::Left:
                    return "Operator::Arrow::Left";
                case TokenTypes::Operator::Arrow::Right:
                    return "Operator::Arrow::Left";
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

        inline const char *
        to_string(TokenTypes::Operator::ArithmeticUnary value)
        {
            switch (value)
            {
                case TokenTypes::Operator::ArithmeticUnary::Plus:
                    return "Operator::ArithmeticUnary::Plus";
                case TokenTypes::Operator::ArithmeticUnary::Minus:
                    return "Operator::ArithmeticUnary::Minus";
            }

            return "Operator::ArithmeticUnary::Unknown";
        }

        inline const char *to_string(TokenTypes::Operator::Arithmetic value)
        {
            switch (value)
            {
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
                case TokenTypes::Operator::Relational::GreatherThanEqual:
                    return "Operator::Relational::GreatherThanEqual";
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

    } // namespace TokenTypes

    using TokenType = std::variant<
        TokenTypes::Reserved, TokenTypes::Primary, TokenTypes::Operator::Dot,
        TokenTypes::Operator::Arrow, TokenTypes::Operator::Assignment,
        TokenTypes::Operator::ArithmeticUnary, TokenTypes::Operator::Arithmetic,
        TokenTypes::Operator::Unary, TokenTypes::Operator::Logical,
        TokenTypes::Operator::Relational, TokenTypes::Delimeter,
        TokenTypes::Miscellaneous>;

    inline const char *type_to_string(TokenType type)
    {
        return std::visit([&](auto &&value)
                          { return TokenTypes::to_string(value); }, type);
    }

} // namespace Lexer
