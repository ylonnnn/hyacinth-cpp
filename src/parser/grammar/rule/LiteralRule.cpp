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
        ParseResult result;

        Lexer::Lexer &lexer = parser.lexer();
        if (lexer.eof())
            return result;
        // Utils::terminate("Lexer has reached the EOF!", EXIT_FAILURE);

        Lexer::Token *token = lexer.next();

        // std::cout << *token << " | " << Lexer::type_to_string(token_type_)
        //           << "\n";

        if (token->type == token_type_)
        {
            parser.update_state(ParserState::Synchronized);
            return result;
        }

        if (parser.state() == ParserState::Panic)
            return result;

        result.errors.push_back(Diagnostic::ErrorDiagnostic(
            std::make_unique<AST::LiteralExpr>(*token),
            Diagnostic::ErrorTypes::General::Syntax,

            // Error Message
            std::string() + "Unexpected \"" + Utils::Colors::Red +
                std::string(token->value) + Utils::Styles::Reset +
                "\". Expected " + Utils::Colors::Red +
                Lexer::type_to_string(token_type_) + Utils::Styles::Reset,

            // Emphasis Label
            std::string("Received ") + Lexer::type_to_string(token->type) +
                " instead."));

        std::cout << "failed matching: " << *token << "\n";
        parser.update_state(ParserState::Panic);

        return result;
    }

} // namespace Parser
