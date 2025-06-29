#pragma once

#include "ast/Node.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/rules/common/Mutability.hpp"
#include "parser/grammar/rules/common/Terminator.hpp"

namespace Parser
{
    class IdentifierNode : public AST::Node
    {
      private:
        Lexer::Token &name_;
        bool mutable_;
        // std::unique_prt<AST::Expr> type_;

      public:
        IdentifierNode(Lexer::Token &name, bool mut);

        Lexer::Token &name();

        bool is_mutable() const;

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
