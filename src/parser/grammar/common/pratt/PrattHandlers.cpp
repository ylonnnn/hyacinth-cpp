#include <memory>

#include "ast/NodeCollection.hpp"
#include "ast/common/Identifier.hpp"
#include "ast/expr/ArrayLiteralExpr.hpp"
#include "ast/expr/Path.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/pratt/Pratt.hpp"
#include "parser/grammar/common/pratt/PrattHandlers.hpp"
#include "utils/dev.hpp"
#include "utils/pointer.hpp"

namespace Parser
{

    NudHandler<AST::Node> make_group_handler(Lexer::TokenType &&cl_pair,
                                             GroupHandlerFn &&handler)
    {
        return [&, cl_pair, handler = std::move(handler)](
                   Parser &parser,
                   ParseResult &result) -> std::unique_ptr<AST::Node>
        {
            auto &lexer = parser.lexer;
            lexer.consume();

            ParseResult p_res = handler(parser);
            result.adapt(p_res.status, std::move(p_res.diagnostics));

            parser.expect_or_error(cl_pair, result,
                                   ParserTokenConsumptionType::UponSuccess);

            return std::move(p_res.data);
        };
    }

    // Default

    std::unique_ptr<AST::LiteralExpr> parse_literal(Parser &parser,
                                                    ParseResult &)
    {
        auto &lexer = parser.lexer;
        Lexer::Token *token = lexer.peek();

        if (token == nullptr)
            return nullptr;

        lexer.consume();

        return std::make_unique<AST::LiteralExpr>(*token);
    }

    std::unique_ptr<AST::Identifier> parse_identifier(Parser &parser,
                                                      ParseResult &result)
    {
        return Common::PathRule.parse_ident(parser, result);
    }

    std::unique_ptr<AST::Path> parse_path(Parser &parser,
                                          std::unique_ptr<AST::Node> &left,
                                          float, ParseResult &result)
    {
        auto left_ = utils::dynamic_ptr_cast<AST::Path>(left);
        if (left_ == nullptr)
            return nullptr;

        Common::PathRule.parse_path(parser, left_, result);
        return left_;
    }

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Node> &left,
                 float right_bp, ParseResult &result)
    {
        auto left_ = utils::dynamic_ptr_cast<AST::Expr>(left);
        if (left_ == nullptr)
            return nullptr;

        auto &lexer = parser.lexer;
        Lexer::Token *operation = lexer.peek();

        if (operation == nullptr)
            return nullptr;

        lexer.consume();

        ParseResult r_res = Common::Pratt.parse_base(parser, right_bp);
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

        return std::make_unique<AST::BinaryExpr>(*operation, std::move(left_),
                                                 std::move(right_));
    }

    std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser, ParseResult &)
    {
        auto &lexer = parser.lexer;
        Lexer::Token *operation = lexer.peek();

        if (operation == nullptr)
            return nullptr;

        lexer.consume();

        Pratt &pratt = Common::Pratt;
        PrattHandler *handler = pratt.get_handler(operation->type);

        if (handler == nullptr)
            return nullptr;

        ParseResult v_res = pratt.parse_base(parser, handler->bp.second);
        auto left_ = utils::dynamic_ptr_cast<AST::Expr>(v_res.data);

        if (left_ == nullptr)
            return nullptr;

        return std::make_unique<AST::UnaryExpr>(AST::UnaryType::Pre, *operation,
                                                std::move(left_));
    }

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Node> &left, float,
                ParseResult &)
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

    NudHandler<AST::Node> make_grouped_expr_handler(Lexer::TokenType &&cl_pair)
    {
        return make_group_handler(
            std::move(cl_pair),
            [cl_pair](Parser &parser) -> ParseResult
            {
                using Lexer::TokenType;

                auto &lexer = parser.lexer;
                ParseResult result{
                    parser, Core::ResultStatus::Success, nullptr, {}};

                auto collection =
                    std::make_unique<AST::NodeCollection<AST::Expr>>(
                        lexer.current().range.start(),
                        std::vector<std::unique_ptr<AST::Expr>>{});

                auto expect_arg = true;

                while (!parser.expect(cl_pair, false))
                {
                    if (expect_arg)
                    {
                        ParseResult p_res = Common::Pratt.parse_base(parser, 0);
                        result.adapt(p_res.status,
                                     std::move(p_res.diagnostics));

                        if (p_res.data == nullptr)
                            break;

                        auto value =
                            dynamic_cast<AST::Expr *>(p_res.data.get());
                        if (value == nullptr)
                            break;

                        // Store and use to avoid warnings
                        auto _ = p_res.data.release();
                        (void)_;

                        collection->collection.emplace_back(value);
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

                if (auto cl = parser.expect_or_error(
                        cl_pair, result, ParserTokenConsumptionType::Preserve))
                    collection->range.end(cl->range.end());

                result.data = std::move(collection);

                return result;
            });
    }

    std::unique_ptr<AST::FunctionCallExpr>
    parse_func_call(Parser &parser, std::unique_ptr<AST::Node> &left, float,
                    ParseResult &result)
    {
        auto callee = utils::dynamic_ptr_cast<AST::Expr>(left);
        if (callee == nullptr)
        {
            utils::todo("throw error: cannot invoke non-expression nodes");
            return nullptr;
        }

        std::unique_ptr<AST::Node> args = make_grouped_expr_handler(
            Lexer::TokenType::LeftParen)(parser, result);

        auto arguments =
            utils::dynamic_ptr_cast<AST::NodeCollection<AST::Expr>>(args);
        if (arguments == nullptr)
            return nullptr;

        return std::make_unique<AST::FunctionCallExpr>(
            std::move(callee), std::move(arguments->collection));
    }

    std::unique_ptr<AST::ArrayLiteralExpr>
    parse_array_literal_expr(Parser &parser, ParseResult &result)
    {
        Lexer::Lexer &lexer = parser.lexer;

        Core::Position &start_pos = lexer.peek()->range.start();
        size_t prev = lexer.position;

        ParseResult t_res = Common::Pratt.parse_base(parser, 0, true);
        std::unique_ptr<AST::PrefixedType> type;

        if (t_res.data == nullptr)
        {
            lexer.position = prev + 1; // Revert next to the last valid position
            Lexer::Token *p_token =
                parser.expect_or_error(Lexer::TokenType::RightBracket, result);

            if (p_token == nullptr)
                return nullptr;
        }

        else
        {
            type = utils::dynamic_ptr_cast<AST::PrefixedType>(t_res.data);
            if (type == nullptr || type->kind != AST::PrefixedTypeKind::Array)
            {
                utils::todo("throw error: cannot construct array literal from "
                            "non-array type");

                return nullptr;
            }

            result.adapt(t_res.status, std::move(t_res.diagnostics));
        }

        // {...}
        std::unique_ptr<AST::Node> values = make_grouped_expr_handler(
            Lexer::TokenType::RightBrace)(parser, result);

        auto elements =
            utils::dynamic_ptr_cast<AST::NodeCollection<AST::Expr>>(values);
        if (elements == nullptr)
            return nullptr;

        auto node = std::make_unique<AST ::ArrayLiteralExpr>(start_pos,
                                                             std::move(type));
        node->elements.swap(elements->collection);

        return node;
    }

    // Type

    std::unique_ptr<AST::SimpleType> parse_type_identifier(Parser &parser,
                                                           ParseResult &result)
    {
        std::unique_ptr<AST::Path> path =
            Common::PathRule.parse_path(parser, result);

        return std::make_unique<AST::SimpleType>(std::move(path));
    }

    std::unique_ptr<AST::PrefixedType> parse_type_array(Parser &parser,
                                                        ParseResult &result)
    {
        auto &lexer = parser.lexer;
        Core::Position &st_pos = lexer.next()->range.start(); // Consume [

        // Validate ]
        parser.expect_or_error(Lexer::TokenType::RightBracket, result,
                               ParserTokenConsumptionType::Preserve);

        auto nud = make_type_pref_nud_handler(TypeBindingPower::Array,
                                              AST::PrefixedTypeKind::Array);

        auto node = nud(parser, result);
        node->range.start(st_pos);

        return node;
    }

    NudHandler<AST::PrefixedType>
    make_type_pref_nud_handler(TypeBindingPower bp, AST::PrefixedTypeKind kind)
    {
        return [&, bp,
                kind](Parser &parser,
                      ParseResult &result) -> std::unique_ptr<AST::PrefixedType>
        {
            Core::Position &ts_pos = parser.lexer.next()->range.start();

            ParseResult b_res = Common::Pratt.parse_base(
                parser, static_cast<int32_t>(bp), true);
            result.adapt(b_res.status, std::move(b_res.diagnostics));

            auto base = utils::dynamic_ptr_cast<AST::Type>(b_res.data);
            if (base == nullptr)
                return nullptr;

            auto type =
                std::make_unique<AST::PrefixedType>(kind, std::move(base));
            type->range.start(ts_pos);

            return type;
        };
    }

    LedHandler<AST::SuffixedType>
    make_type_suf_led_handler(TypeBindingPower bp, AST::SuffixedTypeKind kind)
    {
        return [&, bp,
                kind](Parser &parser, std::unique_ptr<AST::Node> &left, float,
                      ParseResult &) -> std::unique_ptr<AST::SuffixedType>
        {
            parser.lexer.consume();

            auto base = utils::dynamic_ptr_cast<AST::Type>(left);
            if (base == nullptr)
                return nullptr;

            return std::make_unique<AST::SuffixedType>(kind, std::move(base));
        };
    }

    NudHandler<AST::ModifiedType>
    make_type_mod_nud_handler(TypeBindingPower bp, AST::ModifierType type)
    {
        return [&, bp,
                type](Parser &parser,
                      ParseResult &result) -> std::unique_ptr<AST::ModifiedType>
        {
            Core::Position &ts_pos = parser.lexer.next()->range.start();

            ParseResult b_res = Common::Pratt.parse_base(
                parser, static_cast<int32_t>(bp), true);
            result.adapt(b_res.status, std::move(b_res.diagnostics));

            auto base = utils::dynamic_ptr_cast<AST::Type>(b_res.data);
            if (base == nullptr)
                return nullptr;

            auto m_type =
                std::make_unique<AST::ModifiedType>(type, std::move(base));
            m_type->range.start(ts_pos);

            return m_type;
        };
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

    //     node->set_range.end(*c_ep);

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
    //         fnc_ep = &lexer.next()->range.end;

    //     // return std::make_unique<AST::BinaryExpr>(
    //     //     std::move(left), operation,
    //     //     Common::Expr.parse_expr(parser, right_bp).data);

    //     auto node = std::make_unique<AST::FunctionCallExpr>(
    //         std::move(left), std::move(arguments));

    //     node->set_range.end(*fnc_ep);

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

    //     node->set_range.end(*c_ep);

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
    //         c_ep = &lexer.next()->range.end;

    //     if (left == nullptr)
    //     {
    //         auto node = std::make_unique<AST::InstanceExpr>(opening.position,
    //                                                         std::move(fields));

    //         node->set_range.end(*c_ep);

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

    //     node->set_range.end(*c_ep);

    //     return node;
    // }

} // namespace Parser
