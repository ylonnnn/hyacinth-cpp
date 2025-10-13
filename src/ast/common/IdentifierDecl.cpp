#include "ast/common/IdentifierDecl.hpp"
#include "ast/Node.hpp"
#include "utils/style.hpp"

namespace AST
{
    IdentifierDecl::IdentifierDecl(Lexer::Token &identifier,
                                   IdentifierMutabilityState mut_state,
                                   std::unique_ptr<Type> type)
        : Node(identifier.range.start), mut_state(mut_state),
          identifier(identifier), type(std::move(type))
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
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "Identifier {\n"
           << inner_indentation << "identifier: " << identifier << "\n"
           << inner_indentation << "mut_state: " << mut_state << "\n"
           << inner_indentation << "type: ";

        type->print(os, 4);

        os << "\n" << indentation << "}";
    }

} // namespace AST
