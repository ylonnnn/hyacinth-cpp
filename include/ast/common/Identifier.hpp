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

    class Identifier : virtual public Node
    {
      protected:
        Lexer::Token &name_;
        IdentifierMutabilityState mut_state_;
        std::unique_ptr<Type> type_;

      public:
        Identifier(Lexer::Token &name, IdentifierMutabilityState mut_state,
                   std::unique_ptr<Type> type);

        Lexer::Token &name();
        IdentifierMutabilityState mut_state() const;
        Type &type();

        std::unique_ptr<Type> &type_ptr();

        bool is_mutable() const;

        virtual void print(std::ostream &os, uint8_t tab) const override = 0;
    };

} // namespace AST
