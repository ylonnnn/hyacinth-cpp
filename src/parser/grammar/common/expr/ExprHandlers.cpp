#include <memory>

#include "ast/expr/TypeExpr.hpp"
#include "ast/type/SimpleType.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/expr/Expr.hpp"
#include "parser/grammar/common/expr/ExprHandlers.hpp"

namespace Parser
{
    std::unique_ptr<AST::LiteralExpr> parse_literal(Parser &parser,
                                                    ExprParseResult &)
    {
        return std::make_unique<AST::LiteralExpr>(parser.lexer().current());
    }

    std::unique_ptr<AST::Expr> parse_idtype_expr(Parser &parser,
                                                 ExprParseResult &__res)
    {
        auto &lexer = parser.lexer();

        lexer.rewind(lexer.position() - 1);
        Lexer::Token *p_token = lexer.peek();

        TypeParseResult result = Common::Type.parse_type(parser);
        Lexer::Token *n_token = lexer.peek();

        AST::Type *type = result.data.get();

        if (typeid(*type) == typeid(AST::SimpleType))
            return std::make_unique<AST::IdentifierExpr>(*p_token);

        LedHandler led = get_led(n_token->type);
        if (led == nullptr)
            return parse_identifier(parser, __res);

        __res.data = std::make_unique<AST::TypeExpr>(std::move(result.data));
        lexer.next();

        return led(parser, __res.data, 0, __res);
    }

    std::unique_ptr<AST::IdentifierExpr> parse_identifier(Parser &parser,
                                                          ExprParseResult &)
    {
        Lexer::Token &token = parser.lexer().current();

        return std::make_unique<AST::IdentifierExpr>(token);
    }

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                 float right_bp, ExprParseResult &result)
    {
        auto &lexer = parser.lexer();
        Lexer::Token &operation = lexer.current();

        Expr *expr_rule = dynamic_cast<Expr *>(parser.grammar().fallback());
        if (expr_rule == nullptr)
            return nullptr;

        ExprParseResult r_res = expr_rule->parse_expr(parser, right_bp);

        std::unique_ptr<AST::Expr> right = std::move(r_res.data);
        result.adapt(r_res.status, std::move(r_res.diagnostics));

        if (right == nullptr)
        {
            auto node = AST::LiteralExpr(lexer.current());
            result.force_error(&node,
                               Diagnostic::ErrorTypes::Syntax::MissingValue,
                               std::string("Missing ") + Diagnostic::ERR_GEN +
                                   "VALUE" + Utils::Styles::Reset +
                                   " for the right-side of the expression",
                               "Missing here");
        }

        return std::make_unique<AST::BinaryExpr>(std::move(left), operation,
                                                 std::move(right));
    }

    std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
                                                ExprParseResult &)
    {
        Lexer::Token &operation = parser.lexer().current();

        Expr *expr_rule = dynamic_cast<Expr *>(parser.grammar().fallback());
        if (expr_rule == nullptr)
            return nullptr;

        return std::make_unique<AST::UnaryExpr>(
            AST::UnaryType::Pre, operation,
            expr_rule->parse_expr(parser, 0).data);
    }

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Expr> &left, float,
                ExprParseResult &)
    {
        return std::make_unique<AST::UnaryExpr>(
            AST::UnaryType::Post, parser.lexer().current(), std::move(left));
    }

    std::unique_ptr<AST::BinaryExpr>
    parse_memaccess(Parser &parser, std::unique_ptr<AST::Expr> &left,
                    float right_bp, ExprParseResult &result)
    {
        auto &lexer = parser.lexer();
        Lexer::Token &operation = lexer.current();

        Expr *expr_rule = dynamic_cast<Expr *>(parser.grammar().fallback());
        if (expr_rule == nullptr)
            return nullptr;

        ExprParseResult r_res = expr_rule->parse_expr(parser, right_bp);

        std::unique_ptr<AST::Expr> right = std::move(r_res.data);
        result.adapt(r_res.status, std::move(r_res.diagnostics));

        if (right == nullptr)
        {
            auto node = AST::LiteralExpr(lexer.current());
            result.force_error(&node,
                               Diagnostic::ErrorTypes::Syntax::MissingValue,
                               std::string("Missing ") + Diagnostic::ERR_GEN +
                                   "ACCESSOR" + Utils::Styles::Reset +
                                   " for the the member access expression",
                               "Accessing nothing after the dot operator");
        }

        return std::make_unique<AST::BinaryExpr>(std::move(left), operation,
                                                 std::move(right));
    }

    std::unique_ptr<AST::FunctionCalLExpr>
    parse_fncall(Parser &parser, std::unique_ptr<AST::Expr> &left, float,
                 ExprParseResult &result)
    {
        auto &lexer = parser.lexer();

        Expr *expr_rule = dynamic_cast<Expr *>(parser.grammar().fallback());
        if (expr_rule == nullptr)
            return nullptr;

        std::vector<std::unique_ptr<AST::Expr>> arguments;
        arguments.reserve(8);

        auto expect_arg = true;
        Lexer::TokenType closing_token_type =
            Lexer::TokenTypes::Delimeter::ParenthesisClose;

        while (!parser.expect(closing_token_type, false))
        {
            if (expect_arg)
            {
                ExprParseResult expr_res = expr_rule->parse_expr(parser, 0);

                if (expr_res.data == nullptr)
                {
                    result.error(
                        Diagnostic::create_syntax_error(&lexer.current()));

                    return nullptr;
                }

                arguments.push_back(std::move(expr_res.data));
                expect_arg = false;
            }

            if (parser.expect(Lexer::TokenTypes::Delimeter::Comma, false))
            {
                lexer.next();
                expect_arg = true;

                if (parser.expect(closing_token_type, false))
                    break;

                continue;
            }

            break;
        }

        Core::Position *fnc_ep = nullptr;
        if (auto diagnostic = parser.expect_or_error(closing_token_type, false))
        {
            parser.panic();

            result.status = Core::ResultStatus::Fail;
            result.diagnostics.push_back(std::move(diagnostic));
        }

        else
            fnc_ep = &lexer.next()->end_position;

        // return std::make_unique<AST::BinaryExpr>(
        //     std::move(left), operation,
        //     expr_rule->parse_expr(parser, right_bp).data);

        auto node = std::make_unique<AST::FunctionCalLExpr>(
            std::move(left), std::move(arguments));

        node->set_end_position(*fnc_ep);

        return node;
    }

    std::unique_ptr<AST::InstanceExpr>
    parse_instance(Parser &parser, std::unique_ptr<AST::Expr> &left, float,
                   ExprParseResult &result)
    {
        using namespace Lexer::TokenTypes;
        auto &lexer = parser.lexer();

        Expr *expr_rule = dynamic_cast<Expr *>(parser.grammar().fallback());
        if (expr_rule == nullptr)
            return nullptr;

        auto &opening = lexer.current();

        std::unordered_map<std::string, AST::InstanceField> fields;
        fields.reserve(8);

        auto expect_field = true;
        auto closing = Lexer::TokenTypes::Delimeter::BraceClose;

        while (!parser.expect(closing, false))
        {
            if (expect_field)
            {
                Lexer::Token *field = nullptr;
                std::string field_name;

                if (auto diagnostic =
                        parser.expect_or_error(Primary::Identifier, false))
                    result.error(std::move(diagnostic));
                else
                {
                    field = lexer.next();
                    field_name = std::string(field->value);
                }

                if (auto diagnostic =
                        parser.expect_or_error(Delimeter::Colon, false))
                    result.error(std::move(diagnostic));
                else
                    lexer.next();

                ExprParseResult expr_res = expr_rule->parse_expr(parser, 0);
                if (expr_res.data == nullptr)
                {
                    result.error(
                        Diagnostic::create_syntax_error(&lexer.current()));

                    return nullptr;
                }

                auto [_, inserted] = fields.try_emplace(
                    field_name,
                    AST::InstanceField{*field, std::move(expr_res.data)});

                // Duplication
                if (!inserted)
                {
                    auto node = AST::LiteralExpr(*field);
                    result.error(
                        &node, Diagnostic::ErrorTypes::Syntax::FieldDuplcation,
                        std::string("Field \"") + Diagnostic::ERR_GEN +
                            field_name + Utils::Styles::Reset +
                            "\" has already been defined.",
                        "Field is being duplicated");

                    break;
                }

                expect_field = false;
            }

            if (parser.expect(Lexer::TokenTypes::Delimeter::Comma, false))
            {
                lexer.next();
                expect_field = true;

                // Closing Brace Lookahead
                if (parser.expect(closing, false))
                    break;

                continue;
            }

            break;
        }

        Core::Position *c_ep = nullptr;
        if (auto diagnostic = parser.expect_or_error(closing, false))
        {
            parser.panic();
            result.adapt(Core::ResultStatus::Fail,
                         Diagnostic::DiagnosticList{
                             std::make_move_iterator(&diagnostic),
                             std::make_move_iterator(&diagnostic + 1)});
        }

        else
            c_ep = &lexer.next()->end_position;

        if (left == nullptr)
        {
            auto node = std::make_unique<AST::InstanceExpr>(opening.position,
                                                            std::move(fields));

            node->set_end_position(*c_ep);

            return node;
        }

        std::unique_ptr<AST::Type> type;

        AST::Expr *l_expr = left.get();
        auto &t_info = typeid(*l_expr);

        if (t_info == typeid(AST::IdentifierExpr))
            type = std::make_unique<AST::SimpleType>(
                static_cast<AST::IdentifierExpr *>(l_expr)->identifier());

        else if (t_info == typeid(AST::TypeExpr))
            type = std::move(
                static_cast<AST::TypeExpr *>(left.release())->type_ptr());

        auto node = std::make_unique<AST::InstanceExpr>(std::move(type),
                                                        std::move(fields));

        node->set_end_position(*c_ep);

        return node;
    }

} // namespace Parser
