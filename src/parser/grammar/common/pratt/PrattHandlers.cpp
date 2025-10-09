#include <memory>

// #include "ast/expr/TypeExpr.hpp"
// #include "ast/type/SimpleType.hpp"
// #include "diagnostic/ErrorDiagnostic.hpp"
// #include "parser/grammar/common/Common.hpp"
#include "ast/common/Identifier.hpp"
#include "ast/expr/Path.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/pratt/Pratt.hpp"
#include "parser/grammar/common/pratt/PrattHandlers.hpp"
#include "utils/dev.hpp"
#include "utils/pointer.hpp"

namespace Parser
{
    std::unique_ptr<AST::LiteralExpr> parse_literal(Parser &parser,
                                                    PrattParseResult &)
    {
        auto &lexer = parser.lexer;
        Lexer::Token *token = lexer.peek();

        if (token == nullptr)
            return nullptr;

        lexer.consume();

        return std::make_unique<AST::LiteralExpr>(*token);
    }

    std::unique_ptr<AST::Identifier>
    parse_identifier(Parser &parser, [[maybe_unused]] PrattParseResult &result)
    {
        return Common::PathRule.parse_ident(parser);
    }

    std::unique_ptr<AST::Path> parse_path(Parser &parser,
                                          std::unique_ptr<AST::Node> &left,
                                          float right_bp,
                                          PrattParseResult &result)
    {
        auto left_ = utils::dynamic_ptr_cast<AST::Path>(left);
        if (left_ == nullptr)
            return nullptr;

        Common::PathRule.parse_path(parser, left_);
        return left_;
    }

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Node> &left,
                 float right_bp, PrattParseResult &result)
    {
        auto left_ = utils::dynamic_ptr_cast<AST::Expr>(left);
        if (left_ == nullptr)
            return nullptr;

        auto &lexer = parser.lexer;
        Lexer::Token *operation = lexer.peek();

        if (operation == nullptr)
            return nullptr;

        lexer.consume();

        PrattParseResult r_res = Common::Pratt.parse_base(parser, right_bp);
        result.adapt(r_res.status, std::move(r_res.diagnostics));

        auto right_ = utils::dynamic_ptr_cast<AST::Expr>(r_res.data);
        if (right_ == nullptr)
            utils::todo("throw error: missing right-side value");

        // if (right == nullptr)
        // {
        //     auto node = AST::LiteralExpr(lexer.current());
        //     result.force_error(&node,
        //                        Diagnostic::ErrorTypes::Syntax::MissingValue,
        //                        std::string("Missing ") + Diagnostic::ERR_GEN
        //                        +
        //                            "VALUE" + utils::Styles::Reset +
        //                            " for the right-side of the
        //                            expression ",
        //                            "Missing here");
        // }

        return std::make_unique<AST::BinaryExpr>(std::move(left_), *operation,
                                                 std::move(right_));
    }

    std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
                                                PrattParseResult &result)
    {
        auto &lexer = parser.lexer;
        Lexer::Token *operation = lexer.peek();

        if (operation == nullptr)
            return nullptr;

        lexer.consume();
        PrattParseResult v_res = Common::Pratt.parse_base(parser, 0);

        auto left_ = utils::dynamic_ptr_cast<AST::Expr>(v_res.data);
        if (left_ == nullptr)
            return nullptr;

        return std::make_unique<AST::UnaryExpr>(AST::UnaryType::Pre, *operation,
                                                std::move(left_));
    }

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Node> &left,
                float right_bp, PrattParseResult &result)
    {
        auto left_ = utils::dynamic_ptr_cast<AST::Expr>(left);
        if (left_ == nullptr)
            return nullptr;

        auto &lexer = parser.lexer;
        Lexer::Token *operation = lexer.peek();

        if (operation == nullptr)
            return nullptr;

        lexer.consume();

        return std::make_unique<AST::UnaryExpr>(AST::UnaryType::Post,
                                                *operation, std::move(left_));
    }

    // std::unique_ptr<AST::BinaryExpr>
    // parse_memaccess(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //                 float right_bp, ExprParseResult &result)
    // {
    //     auto &lexer = parser.lexer();
    //     Lexer::Token &operation = lexer.current();

    //     ExprParseResult r_res = Common::Expr.parse_expr(parser, right_bp);

    //     std::unique_ptr<AST::Expr> right = std::move(r_res.data);
    //     result.adapt(r_res.status, std::move(r_res.diagnostics));

    //     if (right == nullptr)
    //     {
    //         auto node = AST::LiteralExpr(lexer.current());
    //         result.force_error(&node,
    //                            Diagnostic::ErrorTypes::Syntax::MissingValue,
    //                            std::string("Missing ") + Diagnostic::ERR_GEN
    //                            +
    //                                "ACCESSOR" + utils::Styles::Reset +
    //                                " for the the member access expression",
    //                            "Accessing nothing after the dot operator");
    //     }

    //     return std::make_unique<AST::BinaryExpr>(std::move(left), operation,
    //                                              std::move(right));
    // }

    // std::unique_ptr<AST::BinaryExpr>
    // parse_elaccess(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //                float right_bp, ExprParseResult &result)
    // {
    //     auto &lexer = parser.lexer();
    //     Lexer::Token &operation = lexer.current();

    //     ExprParseResult r_res = Common::Expr.parse_expr(parser, right_bp);

    //     std::unique_ptr<AST::Expr> right = std::move(r_res.data);
    //     result.adapt(r_res.status, std::move(r_res.diagnostics));

    //     if (right == nullptr)
    //     {
    //         auto node = AST::LiteralExpr(lexer.current());
    //         result.force_error(&node,
    //                            Diagnostic::ErrorTypes::Syntax::MissingValue,
    //                            std::string("Missing ") + Diagnostic::ERR_GEN
    //                            +
    //                                "INDEX" + utils::Styles::Reset +
    //                                " for the the element access expression",
    //                            "No specified element index to be accessed");
    //     }

    //     Core::Position *c_ep = nullptr;
    //     if (auto diagnostic = parser.expect_or_error(
    //             Lexer::TokenTypes::Delimeter::BracketClose, false))
    //         result.error(std::move(diagnostic));
    //     else
    //         c_ep = &lexer.next()->position;

    //     auto node = std::make_unique<AST::BinaryExpr>(
    //         std::move(left), operation, std::move(right));

    //     node->set_end_position(*c_ep);

    //     return node;
    // }

    // std::unique_ptr<AST::FunctionCallExpr>
    // parse_fncall(Parser &parser, std::unique_ptr<AST::Expr> &left, float,
    //              ExprParseResult &result)
    // {
    //     auto &lexer = parser.lexer();

    //     std::vector<std::unique_ptr<AST::Expr>> arguments;
    //     arguments.reserve(8);

    //     auto expect_arg = true;
    //     Lexer::TokenType closing_token_type =
    //         Lexer::TokenTypes::Delimeter::ParenthesisClose;

    //     while (!parser.expect(closing_token_type, false))
    //     {
    //         if (expect_arg)
    //         {
    //             ExprParseResult expr_res = Common::Expr.parse_expr(parser,
    //             0);

    //             if (expr_res.data == nullptr)
    //             {
    //                 result.error(
    //                     Diagnostic::create_syntax_error(&lexer.current()));

    //                 return nullptr;
    //             }

    //             arguments.push_back(std::move(expr_res.data));
    //             expect_arg = false;
    //         }

    //         if (parser.expect(Lexer::TokenTypes::Delimeter::Comma, false))
    //         {
    //             lexer.next();
    //             expect_arg = true;

    //             if (parser.expect(closing_token_type, false))
    //                 break;

    //             continue;
    //         }

    //         break;
    //     }

    //     Core::Position *fnc_ep = nullptr;
    //     if (auto diagnostic = parser.expect_or_error(closing_token_type,
    //     false))
    //     {
    //         parser.panic();

    //         result.status = Core::ResultStatus::Fail;
    //         result.diagnostics.push_back(std::move(diagnostic));
    //     }

    //     else
    //         fnc_ep = &lexer.next()->end_position;

    //     // return std::make_unique<AST::BinaryExpr>(
    //     //     std::move(left), operation,
    //     //     Common::Expr.parse_expr(parser, right_bp).data);

    //     auto node = std::make_unique<AST::FunctionCallExpr>(
    //         std::move(left), std::move(arguments));

    //     node->set_end_position(*fnc_ep);

    //     return node;
    // }

    // std::unique_ptr<AST::ArrayExpr> parse_array(Parser &parser,
    //                                             ExprParseResult &result)
    // {
    //     using namespace Lexer::TokenTypes;
    //     auto &lexer = parser.lexer();

    //     lexer.rewind(lexer.position() - 1);

    //     // Parse Value
    //     std::vector<std::unique_ptr<AST::Expr>> elements;
    //     elements.reserve(8);

    //     Core::Position *o_p = nullptr;
    //     if (auto diagnostic =
    //             parser.expect_or_error(Delimeter::BraceOpen, false))
    //         result.error(std::move(diagnostic));
    //     else
    //         o_p = &lexer.next()->position;

    //     auto expect_el = true;
    //     auto closing = Delimeter::BraceClose;

    //     while (!parser.expect(closing, false))
    //     {
    //         if (expect_el)
    //         {
    //             ExprParseResult expr_res = Common::Expr.parse_expr(parser,
    //             0); if (expr_res.data == nullptr)
    //             {
    //                 result.error(
    //                     Diagnostic::create_syntax_error(&lexer.current()));

    //                 return nullptr;
    //             }

    //             elements.emplace_back(std::move(expr_res.data));
    //             expect_el = false;
    //         }

    //         if (parser.expect(Delimeter::Comma, false))
    //         {
    //             lexer.next();
    //             expect_el = true;

    //             // Closing Brace Lookahead
    //             if (parser.expect(closing, false))
    //                 break;

    //             continue;
    //         }

    //         break;
    //     }

    //     Core::Position *c_ep = nullptr;
    //     if (auto diagnostic = parser.expect_or_error(closing, false))
    //     {
    //         parser.panic();
    //         result.adapt(Core::ResultStatus::Fail,
    //                      Diagnostic::DiagnosticList{
    //                          std::make_move_iterator(&diagnostic),
    //                          std::make_move_iterator(&diagnostic + 1)});
    //     }

    //     else
    //         c_ep = &lexer.next()->position;

    //     auto node = std::make_unique<AST::ArrayExpr>(*o_p,
    //     std::move(elements));

    //     node->set_end_position(*c_ep);

    //     return node;
    // }

    // std::unique_ptr<AST::InstanceExpr>
    // parse_instance(Parser &parser, std::unique_ptr<AST::Expr> &left, float,
    //                ExprParseResult &result)
    // {
    //     using namespace Lexer::TokenTypes;
    //     auto &lexer = parser.lexer();

    //     auto &opening = lexer.current();

    //     std::unordered_map<std::string, AST::InstanceField> fields;
    //     fields.reserve(8);

    //     auto expect_field = true;
    //     auto closing = Lexer::TokenTypes::Delimeter::BraceClose;

    //     while (!parser.expect(closing, false))
    //     {
    //         if (expect_field)
    //         {
    //             Lexer::Token *field = nullptr;
    //             std::string field_name;

    //             if (auto diagnostic =
    //                     parser.expect_or_error(Primary::Identifier, false))
    //                 result.error(std::move(diagnostic));
    //             else
    //             {
    //                 field = lexer.next();
    //                 field_name = std::string(field->value);
    //             }

    //             if (auto diagnostic =
    //                     parser.expect_or_error(Delimeter::Colon, false))
    //                 result.error(std::move(diagnostic));
    //             else
    //                 lexer.next();

    //             ExprParseResult expr_res = Common::Expr.parse_expr(parser,
    //             0);

    //             if (expr_res.data == nullptr)
    //             {
    //                 result.error(
    //                     Diagnostic::create_syntax_error(&lexer.current()));

    //                 return nullptr;
    //             }

    //             auto [_, inserted] = fields.try_emplace(
    //                 field_name,
    //                 AST::InstanceField{*field, std::move(expr_res.data)});

    //             // Duplication
    //             if (!inserted)
    //             {
    //                 auto node = AST::LiteralExpr(*field);
    //                 result.error(
    //                     &node,
    //                     Diagnostic::ErrorTypes::Syntax::FieldDuplcation,
    //                     std::string("Field \"") + Diagnostic::ERR_GEN +
    //                         field_name + utils::Styles::Reset +
    //                         "\" has already been defined.",
    //                     "Field is being duplicated");

    //                 break;
    //             }

    //             expect_field = false;
    //         }

    //         if (parser.expect(Lexer::TokenTypes::Delimeter::Comma, false))
    //         {
    //             lexer.next();
    //             expect_field = true;

    //             // Closing Brace Lookahead
    //             if (parser.expect(closing, false))
    //                 break;

    //             continue;
    //         }

    //         break;
    //     }

    //     Core::Position *c_ep = nullptr;
    //     if (auto diagnostic = parser.expect_or_error(closing, false))
    //     {
    //         parser.panic();
    //         result.adapt(Core::ResultStatus::Fail,
    //                      Diagnostic::DiagnosticList{
    //                          std::make_move_iterator(&diagnostic),
    //                          std::make_move_iterator(&diagnostic + 1)});
    //     }

    //     else
    //         c_ep = &lexer.next()->end_position;

    //     if (left == nullptr)
    //     {
    //         auto node = std::make_unique<AST::InstanceExpr>(opening.position,
    //                                                         std::move(fields));

    //         node->set_end_position(*c_ep);

    //         return node;
    //     }

    //     std::unique_ptr<AST::Type> type;

    //     AST::Expr *l_expr = left.get();
    //     auto &t_info = typeid(*l_expr);

    //     if (t_info == typeid(AST::IdentifierExpr))
    //         type = std::make_unique<AST::SimpleType>(
    //             static_cast<AST::IdentifierExpr *>(l_expr)->identifier());

    //     else if (t_info == typeid(AST::TypeExpr))
    //         type = std::move(
    //             static_cast<AST::TypeExpr *>(left.release())->type_ptr());

    //     auto node = std::make_unique<AST::InstanceExpr>(std::move(type),
    //                                                     std::move(fields));

    //     node->set_end_position(*c_ep);

    //     return node;
    // }

} // namespace Parser
