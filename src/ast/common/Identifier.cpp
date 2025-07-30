#include "ast/common/Identifier.hpp"
#include "ast/Node.hpp"

namespace AST
{
    Identifier::Identifier(Lexer::Token &name,
                           IdentifierMutabilityState mut_state,
                           std::unique_ptr<Type> type)
        : Node(name.position), name_(name), mut_state_(mut_state),
          type_(std::move(type))
    {
        if (type_ != nullptr)
            set_end_position(type_->end_position());
    }

    Lexer::Token &Identifier::name() { return name_; }

    IdentifierMutabilityState Identifier::mut_state() const
    {
        return mut_state_;
    }

    Type &Identifier::type() { return *type_; }

    bool Identifier::is_mutable() const
    {
        return mut_state_ == IdentifierMutabilityState::Mutable;
    }

    std::unique_ptr<Type> &Identifier::type_ptr() { return type_; }

} // namespace AST
