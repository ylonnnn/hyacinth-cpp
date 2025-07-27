#include <ostream>

#include "lexer/Token.hpp"

namespace Lexer
{
    std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        return os << "<" << type_to_string(token.type) << " "
                  << token.position.row << ":" << token.position.col << "> "
                  << token.value;
    }

} // namespace Lexer
