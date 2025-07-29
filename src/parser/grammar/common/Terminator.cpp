#include "parser/grammar/common/Terminator.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    Terminator::Terminator() : GrammarRule(Hyacinth::TERMINATOR) {}

    ParseResult Terminator::parse(Parser &parser)
    {
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        if (auto diagnostic = parser.expect_or_error(token_type_, false))
            result.error(std::move(diagnostic));
        else
        {
            Lexer::Token *term = parser.lexer().next();
            result.data = std::make_unique<AST::LiteralExpr>(*term);
        }

        return result;
    }

} // namespace Parser
