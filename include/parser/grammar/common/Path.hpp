#pragma once

#include "ast/common/Identifier.hpp"
#include "ast/expr/Path.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct PathRule : public GrammarRule
    {
      public:
        PathRule();

        std::unique_ptr<AST::Identifier>
        parse_ident(Parser &parser, ParseResult *result = nullptr);
        std::unique_ptr<AST::Path> parse_path(Parser &parser,
                                              ParseResult *result = nullptr);

        void parse_path(Parser &parser, std::unique_ptr<AST::Path> &left);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;
    };

} // namespace Parser
