#include "parser/grammar/common/Terminator.hpp"
#include "parser/grammar/GrammarContext.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    Terminator::Terminator()
        : GrammarRule(Hyacinth::TERMINATOR, GC_GLOBAL | GC_LOCAL)
    {
    }

    ParseResult Terminator::parse(Parser &parser)
    {
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void Terminator::parse(Parser &parser, ParseResult &result)
    {
        if (auto diagnostic = parser.expect_or_error(token_type, false))
        {
            std::cout << "diagnostic: " << diagnostic.get() << "\n";
            result.error(std::move(diagnostic));
        }

        parser.lexer.consume();
    }

} // namespace Parser
