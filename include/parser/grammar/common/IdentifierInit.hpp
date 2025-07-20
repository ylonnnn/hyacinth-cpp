#pragma once

#include "ast/common/Identifier.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Mutability.hpp"
#include "parser/grammar/common/Terminator.hpp"

namespace Parser
{
    class IdentifierInitNode : public AST::Identifier
    {
      public:
        IdentifierInitNode(Lexer::Token &name,
                           AST::IdentifierMutabilityState mut_state,
                           std::unique_ptr<AST::Type> type);

        void print(std::ostream &os, uint8_t tab) const override;
    };

    class IdentifierInitialization : public GrammarRule
    {
      protected:
        Mutability mutability_;
        Terminator terminator_;

      public:
        IdentifierInitialization();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
