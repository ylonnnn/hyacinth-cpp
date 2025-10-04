#pragma once

// #include "ast/stmt/DeclarationStmt.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct VariableDefinition : GrammarRule
    {
        VariableDefinition();

        // ParseResult parse(Parser &parser,
        //                   AST::DeclarationAccessibility accessibility);
        // ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
