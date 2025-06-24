#include "parser/grammar/rule/ChoiceRule.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/Parser.hpp"

namespace Parser
{
    ChoiceGrammarRuleSet::ChoiceGrammarRuleSet()
        : GrammarRuleSet(Lexer::TokenTypes::Miscellaneous::EndOfFile)
    {
    }

    void ChoiceGrammarRuleSet::initialize_rules() {}

    ParseResult ChoiceGrammarRuleSet::parse(Parser &parser)
    {
        auto [status, nodes, errors] = unbuilt_parse(parser);

        // TODO: Build ChoiceGrammarRuleSet Node

        return {status, nullptr, std::move(errors)};
    }

    ChoiceGrammarRule::ChoiceGrammarRule(
        std::vector<std::unique_ptr<GrammarRuleSet>> &&rule_sets)
        : rule_sets_(std::move(rule_sets))
    {
    }

    ChoiceGrammarRule::ChoiceGrammarRule(
        std::vector<std::vector<std::unique_ptr<GrammarRule>>> &&rules)
        : rule_sets_(rules.size())
    {
        for (size_t i = 0; i < rules.size(); i++)
        {
            auto &rule_set = rule_sets_[i];
            if (rule_set == nullptr)
                rule_set = std::make_unique<ChoiceGrammarRuleSet>();

            for (auto &rule : rules[i])
                rule_set->add_rule(std::move(rule));
        }
    }

    ParseResult ChoiceGrammarRule::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();
        std::vector<std::unique_ptr<Diagnostic::Diagnostic>> diagnostics;

        size_t initial_pos = lexer.position();

        for (auto &rule_set : rule_sets_)
        {
            ParseResult result = rule_set->parse(parser);
            if (result.status == ParseResultStatus::Failed ||
                parser.state() == ParserState::Panic)
            {
                diagnostics.insert(
                    diagnostics.end(),
                    std::make_move_iterator(result.diagnostics.begin()),
                    std::make_move_iterator(result.diagnostics.end()));

                lexer.rewind(initial_pos);
                continue;
            }

            return result;
        }

        lexer.rewind(initial_pos);

        return {ParseResultStatus::Failed, nullptr, std::move(diagnostics)};
    }

} // namespace Parser
