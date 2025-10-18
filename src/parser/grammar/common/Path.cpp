#include "parser/grammar/common/Path.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
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

        auto expect_arg = true;

        while (!parser.expect(TokenType::Greater, false) ||
               !parser.expect(TokenType::GreaterGreater, false))
        {
            if (expect_arg)
            {
                // Expression
                if (parser.expect(TokenType::LeftBrace, false))
                {
                    lexer.consume();

                    ParseResult e_res = Common::Pratt.parse_base(parser, 0);
                    result.adapt(e_res.status, std::move(e_res.diagnostics));

                    if (auto diagnostic = parser.expect_or_error(
                            TokenType::RightBrace, false))
                        result.error(std::move(diagnostic));
                    else
                        lexer.consume();

                    auto ptr = utils::dynamic_ptr_cast<AST::Expr>(e_res.data);
                    if (ptr != nullptr)
                        arguments.push_back(std::move(ptr));
                }

                // Type
                else
                {
                    ParseResult t_res =
                        Common::Pratt.parse_base(parser, 0, true);
                    result.adapt(t_res.status, std::move(t_res.diagnostics));

                    auto ptr = utils::dynamic_ptr_cast<AST::Type>(t_res.data);
                    if (ptr != nullptr)
                        arguments.push_back(std::move(ptr));
                }

                expect_arg = false;
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

        Lexer::Token *identifier =
            parser.expect_or_error(TokenType::Identifier, result);
        if (identifier == nullptr)
            return nullptr;

        auto node = std::make_unique<AST::Identifier>(
            *identifier, std::vector<AST::IdentifierArgument>{});

        // <
        if (parser.expect(TokenType::Less, false))
        {
            lexer.consume();

            // ARGS
            node->arguments = parse_args(parser, result);

            // >
            if (auto diagnostic =
                    parser.expect_or_error(TokenType::Greater, false))
            {
                std::cout << *lexer.peek() << "\n";
                // If the next token is ">>" instead of a single ">"
                if (parser.expect(TokenType::GreaterGreater, false))
                {
                    phase++;

                    if (phase >= 2)
                    {
                        lexer.consume();
                        phase = 0;
                    }

                    if (args_depth >= 2)
                        --args_depth;
                }

                else
                    result.error(std::move(diagnostic));
            }

            else
            {
                lexer.consume();
                --args_depth;
            }
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
  // namespace Parser
