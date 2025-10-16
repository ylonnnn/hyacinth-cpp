#include "parser/grammar/common/Path.hpp"
#include "diagnostic/helpers.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/dev.hpp"
#include "utils/pointer.hpp"

namespace Parser
{
    using Lexer::TokenType;

    PathRule::PathRule()
        : GrammarRule(TokenType::EndOfFile, GC_GLOBAL | GC_LOCAL)
    {
    }

    std::vector<AST::IdentifierArgument>
    PathRule::parse_args(Parser &parser, ParseResult &result)
    {
        auto &lexer = parser.lexer;

        std::vector<AST::IdentifierArgument> arguments;
        arguments.reserve(8);

        auto closing = TokenType::RightBracket;
        auto expect_arg = true;

        while (!parser.expect(closing, false))
        {
            if (expect_arg)
            {
                ParseResult t_res = Common::Pratt.parse_base(parser, 0, true);

                auto adapt_type = [&]() -> void
                {
                    result.adapt(t_res.status, std::move(t_res.diagnostics));
                    arguments.emplace_back(
                        utils::dynamic_ptr_cast<AST::Type>(t_res.data));
                };

                auto adapt_expr = [&](ParseResult &e_res) -> void
                {
                    result.adapt(e_res.status, std::move(e_res.diagnostics));
                    arguments.emplace_back(
                        utils::dynamic_ptr_cast<AST::Expr>(e_res.data));
                };

                // If type parsing succeeds, proceed with parsing other
                // arguments
                if (t_res.status == Core::ResultStatus::Success)
                {
                    adapt_type();
                    expect_arg = false;

                    continue;
                }

                // If type parsing fails, attempt expression parsing
                else
                {
                    // Synchronize to be able to attempt expression parsing
                    parser.synchronize();

                    ParseResult e_res = Common::Pratt.parse_base(parser, 0);

                    // If expression parsing fails, use type parsing result
                    if (e_res.status == Core::ResultStatus::Fail)
                        adapt_type();

                    else
                        adapt_expr(e_res);

                    expect_arg = false;
                }
            }

            if (parser.expect(TokenType::Comma, false))
            {
                lexer.consume();
                expect_arg = true;

                continue;
            }

            break;
        }

        return arguments;
    }

    std::unique_ptr<AST::Identifier> PathRule::parse_ident(Parser &parser,
                                                           ParseResult &result)
    {
        auto &lexer = parser.lexer;

        Lexer::Token *identifier = nullptr;
        if (auto diagnostic =
                parser.expect_or_error(TokenType::Identifier, false))
        {
            result.error(std::move(diagnostic));
            return nullptr;
        }

        else
            identifier = lexer.next();

        auto node = std::make_unique<AST::Identifier>(
            *identifier, std::vector<AST::IdentifierArgument>{});

        // [
        if (parser.expect(TokenType::LeftBracket, false))
        {
            lexer.consume();

            // ARGS
            node->arguments = parse_args(parser, result);

            // ]
            if (auto diagnostic =
                    parser.expect_or_error(TokenType::RightBracket, false))
                result.error(std::move(diagnostic));
            else
                lexer.consume();
        }

        return node;
    }

    std::unique_ptr<AST::Path> PathRule::parse_path(Parser &parser,
                                                    ParseResult &result)
    {
        std::vector<std::unique_ptr<AST::Identifier>> segments;
        std::unique_ptr<AST::Identifier> ident = parse_ident(parser, result);

        if (ident == nullptr)
            return nullptr;

        segments.push_back(std::move(ident));

        auto path = std::make_unique<AST::Path>(std::move(segments));

        while (parser.expect(Hyacinth::PATH_SEP, false))
            parse_path(parser, path, result);

        return path;
    }

    void PathRule::parse_path(Parser &parser, std::unique_ptr<AST::Path> &left,
                              ParseResult &result)
    {
        parser.lexer.consume();

        std::unique_ptr<AST::Identifier> ident = parse_ident(parser, result);
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
        result.data = parse_path(parser, result);
    }

} // namespace Parser
