#include <iostream>
#include <vector>

#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/default/Common.hpp"
#include "parser/grammar/rule/LiteralRule.hpp"
#include "utils/style.hpp"

namespace Parser::Hyacinth
{
    MutabilityChoice::MutabilityChoice()
        : ChoiceGrammarRule(std::vector<std::unique_ptr<GrammarRuleSet>>(2))
    {
        for (auto &rule_set : rule_sets_)
            rule_set = std::make_unique<ChoiceGrammarRuleSet>();

        initialize();
    }

    void MutabilityChoice::initialize()
    {
        auto &immutable_rule = rule_sets_[0], &mutable_rule = rule_sets_[1];

        immutable_rule->make_rule<LiteralGrammarRule>(
            Lexer::TokenTypes::Delimeter::Colon);

        mutable_rule->make_rule<LiteralGrammarRule>(
            Lexer::TokenTypes::Operator::Logical::Not);
        mutable_rule->make_rule<LiteralGrammarRule>(
            Lexer::TokenTypes::Delimeter::Colon);
    }

    ParseResult MutabilityChoice::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();
        size_t initial_pos = lexer.position();

        ParseResult result = ChoiceGrammarRule::parse(parser);
        std::cout << "(mutability choice) status: "
                  << static_cast<int>(result.status)
                  << " | position: " << initial_pos << "\n";

        if (result.status == ParseResultStatus::Failed)
        {
            Lexer::Token &token = lexer.current();

            result.diagnostics.clear();
            result.diagnostics.push_back(
                Diagnostic::create_syntax_error(&token));

            // result.diagnostics.push_back(
            //     std::make_unique<Diagnostic::ErrorDiagnostic>(
            //         std::make_unique<AST::LiteralExpr>(token),
            //         Diagnostic::ErrorTypes::General::Syntax,
            //         std::string("Unexpected \"") + Utils::Colors::Red +
            //             std::string(token.value) + Utils::Styles::Reset +
            //             "\".",
            //         std::string("Received ") +
            //             Lexer::type_to_string(token.type) + "."));

            result.diagnostics.push_back(
                std::make_unique<Diagnostic::NoteDiagnostic>(
                    std::make_unique<AST::LiteralExpr>(token),
                    Diagnostic::NoteType::Suggestion,
                    std::string("Invalid modifier \"") + Utils::Colors::Blue +
                        std::string(token.value) + Utils::Styles::Reset +
                        "\". Use \"" + Utils::Colors::Blue + ":" +
                        Utils::Styles::Reset + "\" for immutability or \"" +
                        Utils::Colors::Blue + "!:" + Utils::Styles::Reset +
                        "\" for mutability.",
                    std::string("Invalid modifier token used here.")));
        }

        return result;

        // auto &lexer = parser.lexer();
        // std::vector<std::unique_ptr<Diagnostic::Diagnostic>> diagnostics;

        // size_t initial_pos = lexer.position();

        // for (auto &rule_set : rule_sets_)
        // {
        //     ParseResult result = rule_set->parse(parser);
        //     if (result.status == ParseResultStatus::Failed ||
        //         parser.state() == ParserState::Panic)
        //     {
        //         diagnostics.insert(
        //             diagnostics.end(),
        //             std::make_move_iterator(result.diagnostics.begin()),
        //             std::make_move_iterator(result.diagnostics.end()));

        //         lexer.rewind(initial_pos);
        //         continue;
        //     }

        //     return result;
        // }

        // lexer.rewind(initial_pos);

        // return {ParseResultStatus::Failed, nullptr, std::move(diagnostics)};
    }

} // namespace Parser::Hyacinth
