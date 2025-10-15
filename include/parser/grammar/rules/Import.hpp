#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct ImportStatement : GrammarRule
    {
        ImportStatement();

        std::vector<Lexer::Token *> parse_imported_symbols(Parser &parser,
                                                           ParseResult &result);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

        void recover(Parser &parser) override;
    };

} // namespace Parser
