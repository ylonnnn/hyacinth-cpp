#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct VariableDefinition : GrammarRule
    {
        VariableDefinition();

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

        void recover(Parser &parser) override;
    };

} // namespace Parser
