#pragma once

#include "ast/Node.hpp"
#include "ast/type/Type.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Mutability.hpp"
#include "parser/grammar/common/Terminator.hpp"

namespace Parser
{
    class IdentifierNode : public AST::Node
    {
      protected:
        Lexer::Token &name_;
        bool mutable_;
        std::unique_ptr<AST::Type> type_;

      public:
        IdentifierNode(Lexer::Token &name, bool mut,
                       std::unique_ptr<AST::Type> type);

        Lexer::Token &name();
        std::unique_ptr<AST::Type> &type();

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
