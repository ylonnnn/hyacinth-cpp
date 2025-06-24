#include <iostream>

#include "parser/Parser.hpp"
#include "parser/grammar/default/Common.hpp"
#include "parser/grammar/default/Functions.hpp"
#include "parser/grammar/rule/ChoiceRule.hpp"
#include "parser/grammar/rule/LiteralRule.hpp"
#include "parser/grammar/rule/NonTerminalRule.hpp"
#include "parser/grammar/rule/OptionalRule.hpp"
#include "parser/grammar/rule/RepetitionRule.hpp"

namespace Parser::Hyacinth
{
    FunctionDefinition::FunctionDefinition(Lexer::TokenType token_type)
        : GrammarRuleSet(token_type)
    {
        initialize_rules();
    }

    void FunctionDefinition::initialize_rules()
    {
        make_rule<LiteralGrammarRule>(token_type_);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Primary::Identifier);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Operator::Arrow::Left);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Primary::Identifier);
        make_rule<LiteralGrammarRule>(
            Lexer::TokenTypes::Delimeter::ParenthesisOpen);
        make_rule<OptionalGrammarRule>(
            std::make_unique<FunctionParameterList>());
        make_rule<LiteralGrammarRule>(
            Lexer::TokenTypes::Delimeter::ParenthesisClose);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Delimeter::BraceOpen);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Delimeter::BraceClose);
    }

    ParseResult FunctionDefinition::parse(Parser &parser)
    {
        ParseResult result;
        auto [status, nodes, diagnostics] = unbuilt_parse(parser);

        result.status = status;
        result.diagnostics = std::move(diagnostics);

        // TODO: Build FunctionDefinition Node

        return result;
    }

    FunctionParameterList::FunctionParameterList()
        : GrammarRuleSet(FUNCTION_PARAMETER_LIST)
    {
        initialize_rules();
    }

    void FunctionParameterList::initialize_rules()
    {
        make_rule<NonTerminalGrammarRule>(
            std::make_unique<FunctionParameter>());

        std::vector<std::unique_ptr<GrammarRule>> rules;
        rules.reserve(2);

        rules.push_back(std::make_unique<LiteralGrammarRule>(
            Lexer::TokenTypes::Delimeter::Comma));
        rules.push_back(std::make_unique<NonTerminalGrammarRule>(
            std::make_unique<FunctionParameter>()));

        make_rule<RepetitionGrammarRule>(
            std::move(rules), 0, SIZE_MAX,
            Lexer::TokenTypes::Delimeter::ParenthesisClose,
            Lexer::TokenTypes::Delimeter::BraceOpen);
    }

    ParseResult FunctionParameterList::parse(Parser &parser)
    {
        ParseResult result;
        auto [status, nodes, diagnostics] = unbuilt_parse(parser);

        result.status = status;
        result.diagnostics = std::move(diagnostics);

        // TODO: Build FunctionParameterList Node

        return result;
    }

    FunctionParameter::FunctionParameter() : GrammarRuleSet(FUNCTION_PARAMETER)
    {
        initialize_rules();
    }

    void FunctionParameter::initialize_rules()
    {
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Primary::Identifier);

        // std::vector<std::vector<std::unique_ptr<GrammarRule>>> rules(2);

        // rules[0].push_back(std::make_unique<LiteralGrammarRule>(
        //     Lexer::TokenTypes::Delimeter::Colon));

        // rules[1].reserve(2);
        // rules[1].push_back(std::make_unique<LiteralGrammarRule>(
        //     Lexer::TokenTypes::Operator::Logical::Not));
        // rules[1].push_back(std::make_unique<LiteralGrammarRule>(
        //     Lexer::TokenTypes::Delimeter::Colon));

        // make_rule<ChoiceGrammarRule>(std::move(rules));

        make_rule<MutabilityChoice>();

        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Primary::Identifier);
    }

    ParseResult FunctionParameter::parse(Parser &parser)
    {
        ParseResult result;
        auto [status, nodes, diagnostics] = unbuilt_parse(parser);

        result.status = status;
        result.diagnostics = std::move(diagnostics);

        // TODO: Build FunctionParameter Node

        return result;
    }

}; // namespace Parser::Hyacinth
