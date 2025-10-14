#include "parser/grammar/common/Path.hpp"
#include "diagnostic/helpers.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/dev.hpp"

namespace Parser
{
    using Lexer::TokenType;

    PathRule::PathRule()
        : GrammarRule(TokenType::EndOfFile, GC_GLOBAL | GC_LOCAL)
    {
    }

    std::unique_ptr<AST::Identifier> PathRule::parse_ident(Parser &parser,
                                                           ParseResult *result)
    {
        auto &lexer = parser.lexer;
        Lexer::Token *token = lexer.peek();

        if (token == nullptr)
            return nullptr;

        if (token->type != TokenType::Identifier)
        {
            if (result != nullptr)
                result->error(Diagnostic::create_syntax_error(
                    *token, TokenType::Identifier));

            return nullptr;
        }

        lexer.consume();

        auto identifier = std::make_unique<AST::Identifier>(
            *token, std::vector<AST::IdentifierArgument>{});

        utils::todo("parse generic arguments for identifiers");

        return identifier;
    }

    std::unique_ptr<AST::Path> PathRule::parse_path(Parser &parser,
                                                    ParseResult *result)
    {
        std::vector<std::unique_ptr<AST::Identifier>> segments;
        std::unique_ptr<AST::Identifier> ident = parse_ident(parser, result);

        if (ident == nullptr)
            return nullptr;

        segments.push_back(std::move(ident));

        auto path = std::make_unique<AST::Path>(std::move(segments));

        while (parser.expect(Hyacinth::PATH_SEP, false))
            parse_path(parser, path);

        return path;
    }

    void PathRule::parse_path(Parser &parser, std::unique_ptr<AST::Path> &left)
    {
        parser.lexer.consume();

        std::unique_ptr<AST::Identifier> ident = parse_ident(parser);
        if (ident == nullptr)
            return;

        left->segments.push_back(std::move(ident));
    }

    ParseResult PathRule::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void PathRule::parse(Parser &parser, ParseResult &result)
    {
        result.data = parse_path(parser, &result);
    }

} // namespace Parser
