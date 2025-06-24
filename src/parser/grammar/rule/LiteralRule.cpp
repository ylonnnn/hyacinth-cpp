#include "parser/grammar/rule/LiteralRule.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include <iostream>

namespace Parser
{
    LiteralGrammarRule::LiteralGrammarRule(Lexer::TokenType token_type)
        : token_type_(token_type)
    {
    }

    ParseResult LiteralGrammarRule::parse(Parser &parser)
    {
        ParseResult result = {.status = ParseResultStatus::Failed};

        Lexer::Lexer &lexer = parser.lexer();
        if (lexer.eof())
            return result;

        Lexer::Token *token = lexer.next();

        if (token->type == token_type_)
        {
            result.status = ParseResultStatus::Success;

            if (parser.state() == ParserState::Panic)
                parser.update_state(ParserState::Synchronized);

            return result;
        }

        std::cout << "token: " << *token << " | "
                  << Lexer::type_to_string(token_type_) << "\n";
        std::cout << "status: " << static_cast<int>(result.status)
                  << " | parser.state: " << static_cast<int>(parser.state())
                  << "\n";

        // Skip tokens if the parser is in Panic Mode
        if (parser.state() == ParserState::Panic)
            return result;

        result.status = ParseResultStatus::Failed;
        result.diagnostics.push_back(
            Diagnostic::create_syntax_error(token, token_type_));

        // result.diagnostics.push_back(
        //     std::make_unique<Diagnostic::ErrorDiagnostic>(
        //         std::make_unique<AST::LiteralExpr>(*token),
        //         Diagnostic::ErrorTypes::General::Syntax,

        //         // Error Message
        //         std::string("Unexpected \"") + Utils::Colors::Red +
        //             std::string(token->value) + Utils::Styles::Reset +
        //             "\". Expected " + Utils::Colors::Red +
        //             Lexer::type_to_string(token_type_) +
        //             Utils::Styles::Reset,

        //         // Emphasis Label
        //         std::string("Received ") + Lexer::type_to_string(token->type)
        //         +
        //             " instead."));

        // std::cout << "failed matching: " << *token
        //           << " | expects: " << Lexer::type_to_string(token_type_)
        //           << "\n";

        // Update the state of the parser to Panic Mode to consume unmeaningful
        // tokens
        parser.update_state(ParserState::Panic);

        return result;
    }

} // namespace Parser
