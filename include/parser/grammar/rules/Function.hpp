#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct FunctionParameter : GrammarRule
    {
        FunctionParameter();

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

        void recover(Parser &parser) override;
    };

    struct FunctionDefinition : GrammarRule
    {
        FunctionParameter param_rule;

        FunctionDefinition();

        std::vector<std::unique_ptr<AST::FunctionParameter>>
        parse_parameters(Parser &parser, ParseResult &result);

        Core::Position *parse_closing(Parser &parser, ParseResult &result,
                                      Lexer::TokenType expected);

        ParseResult parse(Parser &parser,
                          AST::DeclarationAccessibility accessibility);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

        void recover(Parser &parser) override;
    };

    struct FunctionDefinitionReturn : GrammarRule
    {
        FunctionDefinitionReturn();

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

        void recover(Parser &parser) override;
    };

} // namespace Parser
