#include <iostream>

#include "ast/common/Identifier.hpp"

namespace AST
{
    Identifier::Identifier(Lexer::Token &identifier,
                           std::vector<IdentifierArgument> &&arguments)
        : Node(identifier.range.start), identifier(identifier),
          arguments(std::move(arguments))
    {
    }

    void Identifier::print(std::ostream &os, uint8_t tab) const
    {
        // TODO: Arguments
        os << "Identifier { identifier: " << identifier << "}";
    }

} // namespace AST
