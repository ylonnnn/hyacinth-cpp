#include <ostream>

#include "lexer/Token.hpp"

namespace Lexer
{
    Token::Token(std::string_view value, Core::PositionRange &&range,
                 TokenType type)
        : value(value), range(std::move(range)), type(type)
    {
    }

    std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        return os << "<" << type_to_name(token.type) << " "
                  << token.range.start.row << ":" << token.range.start.col
                  << "> " << token.value;
    }

} // namespace Lexer
