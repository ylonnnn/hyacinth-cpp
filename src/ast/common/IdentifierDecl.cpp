#include "ast/common/IdentifierDecl.hpp"
#include "ast/Node.hpp"

namespace AST
{
    IdentifierDecl::IdentifierDecl(Lexer::Token &identifier,
                                   IdentifierMutabilityState mut_state,
                                   std::unique_ptr<Type> type)
        : Node(identifier.range.start), identifier(identifier),
          mut_state(mut_state), type(std::move(type))
    {
        if (type != nullptr)
            end_position = type->end_position;
    }

    bool IdentifierDecl::is_mutable() const
    {
        return mut_state == IdentifierMutabilityState::Mutable;
    }

    void IdentifierDecl::print(std::ostream &os, uint8_t tab) const
    {
        std::cout << "Identifier { }\n";
    }

} // namespace AST
