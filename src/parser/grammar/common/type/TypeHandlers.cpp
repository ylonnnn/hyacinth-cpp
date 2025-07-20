#include "parser/grammar/common/type/TypeHandlers.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/common/Common.hpp"

namespace Parser
{
    std::unique_ptr<AST::SimpleType>
    parse_simple(Parser &parser, [[maybe_unused]] TypeParseResult &result)
    {
        return std::make_unique<AST::SimpleType>(parser.lexer().current());
    }

    std::unique_ptr<AST::ConstantType>
    parse_constant(Parser &parser, [[maybe_unused]] TypeParseResult &result)
    {
        return std::make_unique<AST::ConstantType>(parser.lexer().current());
    }

    std::unique_ptr<AST::GenericType>
    parse_generic(Parser &parser, std::unique_ptr<AST::Type> &constructor,
                  [[maybe_unused]] float right_bp, TypeParseResult &result)
    {
        auto &lexer = parser.lexer();

        auto type = std::make_unique<AST::GenericType>(
            std::move(constructor), std::vector<std::unique_ptr<AST::Type>>{});
        type->arguments().reserve(8);

        Lexer::Token &start_token = lexer.current();

        Type &type_rule = Common::Type;
        auto expect_type = true;
        std::vector<std::unique_ptr<AST::Type>> &arguments = type->arguments();

        Lexer::TokenType closing_token =
            Lexer::TokenTypes::Delimeter::BracketClose;

        while (!parser.expect(closing_token, false))
        {
            if (expect_type)
            {
                TypeParseResult type_res = type_rule.parse_type(parser, 0);
                if (type_res.data == nullptr)
                {
                    result.error(
                        Diagnostic::create_syntax_error(&lexer.current()));

                    return nullptr;
                }

                arguments.push_back(std::move(type_res.data));
                expect_type = false;
            }

            if (parser.expect(Lexer::TokenTypes::Delimeter::Comma, false))
            {
                lexer.next();
                expect_type = true;

                continue;
            }

            break;
        }

        if (auto diagnostic = parser.expect_or_error(closing_token, false))
        {
            parser.panic();

            result.status = Core::ResultStatus::Fail;
            result.diagnostics.push_back(std::move(diagnostic));
        }

        else
        {
            Lexer::Token *n_token = lexer.next();

            type->set_end_pos(n_token->position.col + n_token->value.size());
        }

        if (type->arguments().empty())
            result.error(Diagnostic::create_syntax_error(&start_token));

        return type;
    }

    // std::unique_ptr<AST::IdentifierExpr>
    // parse_identifier(Parser &parser, Diagnostic::DiagnosticList
    // &diagnostics);

    // std::unique_ptr<AST::BinaryExpr>
    // parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //              float right_bp, Diagnostic::DiagnosticList &diagnostics);

    // std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
    //                                             Diagnostic::DiagnosticList
    //                                             &diagnostics);
    // std::unique_ptr<AST::UnaryExpr>
    // parse_unary(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //             float right_bp, Diagnostic::DiagnosticList &diagnostics);
} // namespace Parser
