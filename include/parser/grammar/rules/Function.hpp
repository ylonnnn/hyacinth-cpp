#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class FunctionDefinition : public GrammarRule
    {
      public:
        FunctionDefinition();

        ParseResult parse(Parser &parser,
                          AST::DeclarationAccessibility accessibility);
        ParseResult parse(Parser &parser) override;
    };

    class FunctionDefinitionReturn : public GrammarRule
    {
      public:
        FunctionDefinitionReturn();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
