#pragma once

#include "ast/Node.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class MutabilityNode : public AST::Node
    {
      protected:
        bool mutable_;

      public:
        MutabilityNode(Core::Position &position, bool mut);

        void set_mutability(bool mut);
        bool is_mutable() const;

        void print(std::ostream &os, uint8_t tab) const override;
    };

    class Mutability : public GrammarRule
    {
      public:
        Mutability();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
