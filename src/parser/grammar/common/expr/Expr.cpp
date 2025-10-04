#include <unordered_map>
#include <utility>

// #include "ast/stmt/ExprStmt.hpp"
#include "ast/stmt/ExprStmt.hpp"
#include "diagnostic/helpers.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarContext.hpp"
#include "parser/grammar/GrammarRule.hpp"
// #include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/expr/Expr.hpp"
#include "parser/grammar/common/expr/ExprHandlers.hpp"

namespace Parser
{
    ExprParseResult::ExprParseResult(Parser &parser, Core::ResultStatus status,
                                     std::unique_ptr<AST::Expr> data,
                                     Diagnostic::DiagnosticList diagnostics)
        : ParseResult(parser, status, std::move(data), std::move(diagnostics))
    {
        std::unique_ptr<AST::Node> node = std::move(ParseResult::data);
        if (auto ptr = dynamic_cast<AST::Expr *>(node.release()))
            this->data = std::unique_ptr<AST::Expr>(ptr);
    }

    Expr::Expr() : GrammarRule(Lexer::TokenType::EndOfFile, GC_LOCAL)
    {
        initialize();
    }

    void Expr::initialize()
    {
        using namespace Lexer;

        // Literal Expression
        float primary_bp = static_cast<int>(BindingPower::Primary);

        for (const auto &type : std::vector<TokenType>(
                 {TokenType::Int, TokenType::Float, TokenType::Bool,
                  TokenType::Char, TokenType::String}))
        {
            add_handler(type,
                        ExprHandler{.type = type,
                                    .bp = std::pair<float, float>{primary_bp,
                                                                  primary_bp},
                                    .nud = parse_literal,
                                    .led = nullptr});
        }

        // Identifier
        // add_nud(TokenType::Identifier, parse_idtype_expr,
        //         std::pair<float, float>{primary_bp, primary_bp});

        // Grouping
        // add_nud(Delimeter::ParenthesisOpen,
        //         [&](Parser &parser,
        //             ExprParseResult &result) -> std::unique_ptr<AST::Expr>
        //         {
        //             Expr &expr_rule = Common::Expr;

        //             ExprParseResult expr_res = expr_rule.parse_expr(parser,
        //             0); std::unique_ptr<AST::Expr> expr =
        //             std::move(expr_res.data);

        //             result.adapt(expr_res.status,
        //                          std::move(expr_res.diagnostics));

        //             if (auto diagnostic = parser.expect_or_error(
        //                     Delimeter::ParenthesisClose, false))
        //             {
        //                 result.error(std::move(diagnostic));

        //                 return nullptr;
        //             }
        //             else
        //                 parser.lexer().next();

        //             return expr;
        //         });

        // // Compound
        // add_nud(Delimeter::BraceOpen, parse_array);
        // add_led(Delimeter::BraceOpen, parse_instance);

        // // Element Access
        // add_led(Delimeter::BracketOpen, parse_elaccess);

        // // Member Access
        // float memaccess_bp = static_cast<int>(BindingPower::MemberAccess);
        // for (const auto &type : std::vector<Lexer::TokenType>{
        //          Operator::Access::Dot, Operator::Access::DoubleColon})
        // {
        //     add_led(type, parse_memaccess,
        //             std::pair<float, float>{memaccess_bp, memaccess_bp});
        // }

        // // Function Call
        // float fncall_bp = static_cast<int>(BindingPower::FunctionCall);
        // for (const auto &type :
        //      std::vector<Lexer::TokenType>{Delimeter::ParenthesisOpen})
        // {
        //     add_led(type, parse_fncall,
        //             std::pair<float, float>{fncall_bp, fncall_bp});
        // }

        // // Unary
        // float unary_bp = static_cast<int>(BindingPower::Unary);
        // for (const auto &type : std::vector<Lexer::TokenType>({
        //          Unary::Increment,
        //          Unary::Decrement,
        //      }))
        // {
        //     add_nud(
        //         type, [](Parser &parser, ExprParseResult &result)
        //         { return parse_unary(parser, result); },
        //         std::pair<float, float>{unary_bp, unary_bp});

        //     add_led(type, [](Parser &parser, std::unique_ptr<AST::Expr>
        //     &left,
        //                      float right_bp, ExprParseResult &result)
        //             { return parse_unary(parser, left, right_bp, result); });
        // }

        // // Multiplicative
        // float multiplicative_bp =
        //     static_cast<int>(BindingPower::Multiplicative);
        // for (const auto &type : std::vector<Lexer::TokenType>({
        //          Arithmetic::Multiplication,
        //          Arithmetic::Division,
        //          Arithmetic::Modulo,
        //      }))
        // {
        //     add_led(type, parse_binary,
        //             std::pair<float, float>(
        //                 {multiplicative_bp, multiplicative_bp}));
        // }

        // // Additive
        // float additive_bp = static_cast<int>(BindingPower::Additive);
        // for (const auto &type : std::vector<Lexer::TokenType>({
        //          Arithmetic::Plus,
        //          Arithmetic::Minus,
        //      }))
        // {

        //     add_led(type, parse_binary,
        //             std::pair<float, float>{additive_bp, additive_bp});
        // }

        // // Relational
        // float relational_bp = static_cast<int>(BindingPower::Relational);
        // for (const auto &type : std::vector<Lexer::TokenType>({
        //          Relational::Equal,
        //          Relational::NotEqual,
        //          Relational::LessThanEqual,
        //          Relational::LessThan,
        //          Relational::GreaterThanEqual,
        //          Relational::GreaterThan,
        //      }))
        // {
        //     add_led(type, parse_binary,
        //             std::pair<float, float>{relational_bp, relational_bp});
        // }

        // // Logical
        // float logical_bp = static_cast<int>(BindingPower::Logical);
        // for (const auto &type : std::vector<Lexer::TokenType>({
        //          Logical::Not,
        //          Logical::And,
        //      }))
        // {
        //     add_led(type, parse_binary,
        //             std::pair<float, float>{logical_bp, logical_bp});
        // }

        // // Assignment
        // float assignment_bp = static_cast<int>(BindingPower::Assignment);
        // for (const auto &type : std::vector<Lexer::TokenType>({
        //          Assignment::Direct,
        //          Assignment::Addition,
        //          Assignment::Subtraction,
        //          Assignment::Multiplication,
        //          Assignment::Division,
        //          Assignment::Modulo,
        //      }))
        // {
        //     add_led(type, parse_binary,
        //             std::pair<float, float>{assignment_bp, assignment_bp});
        // }
    }

    void Expr::add_handler(Lexer::TokenType type, ExprHandler &&handler)
    {
        handlers_.insert_or_assign(type, std::move(handler));
    }

    ExprHandler *Expr::get_handler(Lexer::TokenType type)
    {
        auto it = handlers_.find(type);
        if (it == handlers_.end())
            return nullptr;

        return &it->second;
    }

    ExprParseResult Expr::parse_expr(Parser &parser, float right_bp)
    {
        ExprParseResult result = {
            parser, Core::ResultStatus::Success, nullptr, {}};

        auto &lexer = parser.lexer;
        if (lexer.eof())
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        Lexer::Token *token = lexer.peek();
        ExprHandler *handler = get_handler(token->type);

        if (handler == nullptr || handler->nud == nullptr)
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        lexer.consume();
        std::unique_ptr<AST::Expr> left = handler->nud(parser, result);

        while (!lexer.eof())
        {
            token = lexer.peek();
            if (token == nullptr)
                break;

            ExprHandler *n_handler = get_handler(token->type);
            if (n_handler == nullptr)
                break;

            auto &[_, bp, __, led] = *n_handler;
            auto &[left_bp_, right_bp_] = bp;

            if (right_bp > left_bp_)
                break;

            // result.status = Core::ResultStatus::Fail;
            // result.diagnostics.push_back(
            //     Diagnostic::create_syntax_error(token));
            if (led == nullptr)
            {
                result.error(Diagnostic::create_syntax_error(*token));
                break;
            }

            lexer.consume();
            left = led(parser, left, right_bp_, result);
        }

        result.data = std::move(left);

        return result;
    }

    ParseResult Expr::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void Expr::parse(Parser &parser, ParseResult &result)
    {
        ExprParseResult expr_result = parse_expr(parser, 0);

        if (expr_result.data == nullptr)
        {
            result.adapt(expr_result.status, std::move(expr_result.diagnostics),
                         nullptr);

            return;
        }

        result.adapt(
            expr_result.status, std::move(expr_result.diagnostics),
            std::make_unique<AST::ExprStmt>(std::move(expr_result.data)));

        if (result.status == Core::ResultStatus::Fail)
        {
            result.data = nullptr;
            return;
        }

        // ; (TERMINATOR)
        Common::Terminator.parse(parser, result);
    }

} // namespace Parser
