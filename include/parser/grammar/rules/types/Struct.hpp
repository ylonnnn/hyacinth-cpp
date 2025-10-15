#pragma once

#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct StructDefinition : GrammarRule
    {
        StructDefinition();

        /**
         * Directly constructs the field inside of the `fields` map provided
         */
        void parse_field(
            Parser &parser, ParseResult &result,
            std::unordered_map<std::string_view, AST::StructField> &fields);

        std::unordered_map<std::string_view, AST::StructField>
        parse_fields(Parser &parser, ParseResult &result);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

        void recover(Parser &parser) override;
    };

} // namespace Parser
