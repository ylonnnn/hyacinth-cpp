#include "parser/grammar/rule/NonTerminalRule.hpp"

namespace Parser
{
    NonTerminalGrammarRule::NonTerminalGrammarRule(
        std::unique_ptr<GrammarRuleSet> rule_set)
        : rule_set_(std::move(rule_set))
    {
    }

    ParseResult NonTerminalGrammarRule::parse(Parser &parser)
    {
        return rule_set_->parse(parser);
    }

} // namespace Parser
