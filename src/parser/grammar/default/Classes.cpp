#include "parser/grammar/default/Classes.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/rule/LiteralRule.hpp"
#include "parser/grammar/rule/RepetitionRule.hpp"
#include <iostream>

namespace Parser::Hyacinth
{
    ClassDefinition::ClassDefinition(Lexer::TokenType token_type)
        : GrammarRuleSet(token_type)
    {
        initialize_rules();
    }

    void ClassDefinition::initialize_rules()
    {
        make_rule<LiteralGrammarRule>(token_type_);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Primary::Identifier);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Delimeter::BraceOpen);
        make_rule<RepetitionGrammarRule>(
            std::make_unique<ClassMemberDefinition>(), 0, SIZE_MAX,
            Lexer::TokenTypes::Delimeter::BraceClose);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Delimeter::BraceClose);
    }

    ParseResult ClassDefinition::parse(Parser &parser)
    {
        ParseResult result;
        auto [status, nodes, diagnostics] = unbuilt_parse(parser);

        result.status = status;
        result.diagnostics = std::move(diagnostics);

        // TODO: Build ClassDefinition Node

        return result;
    }

    ClassMemberDefinition::ClassMemberDefinition()
        : GrammarRuleSet(CLASS_MEMBER)
    {
        initialize_rules();
    }

    void ClassMemberDefinition::initialize_rules()
    {
        make_rule<LiteralGrammarRule>(token_type_);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Delimeter::Colon);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Primary::Identifier);
        make_rule<LiteralGrammarRule>(Lexer::TokenTypes::Delimeter::Semicolon);
    }

    ParseResult ClassMemberDefinition::parse(Parser &parser)
    {
        ParseResult result;
        auto [status, nodes, diagnostics] = unbuilt_parse(parser);

        result.status = status;
        result.diagnostics = std::move(diagnostics);

        // TODO: Build ClassMemberDefinition Node

        return result;
    }

} // namespace Parser::Hyacinth
