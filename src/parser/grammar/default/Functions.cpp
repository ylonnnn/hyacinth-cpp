#include <iostream>

#include "parser/Parser.hpp"
#include "parser/grammar/default/Functions.hpp"
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
        auto [nodes, errors] = unbuilt_parse(parser);

        result.errors = std::move(errors);

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
        make_rule<RepetitionGrammarRule>(
            std::make_unique<FunctionTailingParameter>(), 0, SIZE_MAX,
            Lexer::TokenTypes::Delimeter::ParenthesisClose);
    }

    ParseResult FunctionParameterList::parse(Parser &parser)
    {
        ParseResult result;
        auto [nodes, errors] = unbuilt_parse(parser);

        result.errors = std::move(errors);

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
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Delimeter::Colon);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Primary::Identifier);
    }

    ParseResult FunctionParameter::parse(Parser &parser)
    {
        ParseResult result;
        auto [nodes, errors] = unbuilt_parse(parser);

        result.errors = std::move(errors);

        // TODO: Build FunctionParameter Node

        return result;
    }

    FunctionTailingParameter::FunctionTailingParameter()
        : GrammarRuleSet(FUNCTION_TAILING_PARAMETER)
    {
        initialize_rules();
    }

    void FunctionTailingParameter::initialize_rules()
    {
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Delimeter::Comma);
        make_rule<NonTerminalGrammarRule>(
            std::make_unique<FunctionParameter>());
    }

    ParseResult FunctionTailingParameter::parse(Parser &parser)
    {
        ParseResult result;
        // std::cout << "start of tailing param\n";
        auto [nodes, errors] = unbuilt_parse(parser);
        // std::cout << "end of tailing param\n";

        result.errors = std::move(errors);

        // TODO: Build FunctioNTailingParameter Node

        return result;
    }

}; // namespace Parser::Hyacinth
