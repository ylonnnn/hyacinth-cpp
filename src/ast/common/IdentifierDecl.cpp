#include "ast/common/IdentifierDecl.hpp"
#include "ast/Node.hpp"

namespace AST
{
    IdentifierDecl::IdentifierDecl(Lexer::Token &name,
                                   IdentifierMutabilityState mut_state,
                                   std::unique_ptr<Path> type)
        : Node(name.range.start), name(name), mut_state(mut_state),
          type(std::move(type))
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
