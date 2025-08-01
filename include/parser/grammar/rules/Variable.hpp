#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class VariableDefinition : public GrammarRule
    {
      public:
        VariableDefinition();

        ParseResult parse(Parser &parser,
                          AST::DeclarationAccessibility accessibility);
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
