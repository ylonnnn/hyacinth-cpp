#pragma once

#include "ast/Node.hpp"
#include "ast/type/Type.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    enum IdentifierMutabilityState
    {
        Mutable = 0,
        Immutable,
    };

    struct IdentifierDecl : virtual Node
    {
        IdentifierMutabilityState mut_state;
        Lexer::Token &identifier;
        std::unique_ptr<Type> type;

        IdentifierDecl(Lexer::Token &identifier,
                       IdentifierMutabilityState mut_state,
                       std::unique_ptr<Type> type);

        bool is_mutable() const;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
