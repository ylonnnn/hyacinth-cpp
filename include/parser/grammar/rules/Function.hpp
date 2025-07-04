#pragma once

#include "ast/NodeCollection.hpp"
#include "ast/expr/Expr.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/IdentifierInit.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    class FunctionParameterNode : public IdentifierNode
    {
      protected:
        std::unique_ptr<AST::Expr> default_value_ = nullptr;

      public:
        FunctionParameterNode(
            Lexer::Token &name, bool mut, std::unique_ptr<AST::Type> type,
            std::unique_ptr<AST::Expr> default_value = nullptr);

        std::unique_ptr<AST::Expr> &default_value();

        void print(std::ostream &os, uint8_t tab) const override;
    };

    struct FunctionParameterListParseResult : public ParseResult
    {
        std::unique_ptr<AST::NodeCollection<FunctionParameterNode>> node;
    };

    class FunctionDefinition : public GrammarRule
    {
      public:
        FunctionDefinition();

        FunctionParameterListParseResult parse_param_list(Parser &parser);
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
