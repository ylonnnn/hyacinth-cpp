#pragma once

#include "parser/grammar/GrammarRuleSet.hpp"

namespace Parser::Hyacinth
{
    constexpr auto FUNCTION_PARAMETER_LIST = Lexer::TokenTypes::Primary::Identifier;
    constexpr auto FUNCTION_PARAMETER = Lexer::TokenTypes::Primary::Identifier;
    constexpr auto FUNCTION_TAILING_PARAMETER = Lexer::TokenTypes::Delimeter::Comma;

    class FunctionDefinition : public GrammarRuleSet
    {
      public:
        FunctionDefinition(Lexer::TokenType token_type);

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

    class FunctionParameterList : public GrammarRuleSet
    {
      public:
        FunctionParameterList();

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

    class FunctionParameter : public GrammarRuleSet
    {
      public:
        FunctionParameter();

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

    class FunctionTailingParameter : public GrammarRuleSet
    {
      public:
        FunctionTailingParameter();

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser::Hyacinth
