#include <iostream>

#include "ast/type/GenericType.hpp"
#include "ast/type/SimpleType.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/type/Type.hpp"

namespace Parser
{
    std::unique_ptr<AST::SimpleType>
    parse_simple(Parser &parser, [[maybe_unused]] TypeParseResult &result)
    {
        return std::make_unique<AST::SimpleType>(parser.lexer().current());
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

        while (!parser.expect(
            Lexer::TokenTypes::Operator::Relational::GreaterThan, false))
        {
            if (expect_type)
            {
                TypeParseResult type_res = type_rule.parse_type(parser, 0);
                if (type_res.node == nullptr)
                {
                    result.error(parser, Diagnostic::create_syntax_error(
                                             &lexer.current()));

                    return nullptr;
                }

                type->arguments().push_back(std::move(type_res.node));
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

        if (auto diagnostic = parser.expect_or_error(
                Lexer::TokenTypes::Operator::Relational::GreaterThan, false))
        {
            parser.panic();

            result.status = ParseResultStatus::Failed;
            result.diagnostics.push_back(std::move(diagnostic));
        }

        else
            lexer.next();

        if (type->arguments().empty())
            result.error(parser, Diagnostic::create_syntax_error(&start_token));

        return type;
    }

    // std::unique_ptr<AST::IdentifierExpr>
    // parse_identifier(Parser &parser, DiagnosticList &diagnostics);

    // std::unique_ptr<AST::BinaryExpr>
    // parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //              float right_bp, DiagnosticList &diagnostics);

    // std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
    //                                             DiagnosticList &diagnostics);
    // std::unique_ptr<AST::UnaryExpr>
    // parse_unary(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //             float right_bp, DiagnosticList &diagnostics);
} // namespace Parser
