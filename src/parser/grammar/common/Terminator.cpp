#include "parser/grammar/common/Terminator.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    Terminator::Terminator() : GrammarRule(Hyacinth::TERMINATOR) {}

    ParseResult Terminator::parse(Parser &parser)
    {
        ParseResult result = {ParseResultStatus::Success, nullptr, {}};

        if (auto diagnostic = parser.expect_or_error(token_type_))
        {
            result.status = ParseResultStatus::Failed;
            result.diagnostics.push_back(std::move(diagnostic));
        }

        return result;
    }

} // namespace Parser
