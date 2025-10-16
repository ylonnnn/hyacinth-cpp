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

        std::vector<AST::IdentifierArgument> parse_args(Parser &parser,
                                                        ParseResult &result);

        std::unique_ptr<AST::Identifier> parse_ident(Parser &parser,
                                                     ParseResult &result);
        std::unique_ptr<AST::Path> parse_path(Parser &parser,
                                              ParseResult &result);

        void parse_path(Parser &parser, std::unique_ptr<AST::Path> &left,
                        ParseResult &result);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;
    };

} // namespace Parser
