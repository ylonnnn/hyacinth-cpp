#pragma once

#include "parser/grammar/GrammarRuleSet.hpp"

namespace Parser::Hyacinth
{
    // constexpr auto FUNCTION_PARAMETER_LIST =
    // Lexer::TokenTypes::Primary::Identifier; constexpr auto FUNCTION_PARAMETER
    // = Lexer::TokenTypes::Primary::Identifier; constexpr auto
    // FUNCTION_TAILING_PARAMETER = Lexer::TokenTypes::Delimeter::Comma;

    constexpr auto CLASS_MEMBER = Lexer::TokenTypes::Primary::Identifier;

    class ClassDefinition : public GrammarRuleSet
    {
      public:
        ClassDefinition(Lexer::TokenType token_type);

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

    class ClassMemberDefinition : public GrammarRuleSet
    {
      public:
        ClassMemberDefinition();

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

    // class FunctionParameterList : public GrammarRuleSet
    // {
    //   public:
    //     FunctionParameterList();

    //     void initialize_rules() override;
    //     ParseResult parse(Parser &parser) override;
    // };

    // class FunctionParameter : public GrammarRuleSet
    // {
    //   public:
    //     FunctionParameter();

    //     void initialize_rules() override;
    //     ParseResult parse(Parser &parser) override;
    // };

    // class FunctionTailingParameter : public GrammarRuleSet
    // {
    //   public:
    //     FunctionTailingParameter();

    //     void initialize_rules() override;
    //     ParseResult parse(Parser &parser) override;
    // };

} // namespace Parser::Hyacinth
