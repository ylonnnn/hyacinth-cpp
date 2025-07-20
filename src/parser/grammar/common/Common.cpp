#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/type/Type.hpp"

namespace Parser::Common
{
    using Lexer::TokenTypes::Delimeter;

    ::Parser::Expr Expr;
    ::Parser::Type Type;

    ::Parser::Block Block({Delimeter::BraceOpen, Delimeter::BraceClose});
    ::Parser::IdentifierInitialization IdentifierInitialization;
    ::Parser::Mutability Mutability;
    ::Parser::Terminator Terminator;

} // namespace Parser::Common
