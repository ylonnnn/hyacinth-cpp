#include <iostream>
#include <unordered_map>
#include <utility>

#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/expr/Expr.hpp"
#include "parser/grammar/expr/ExprHandlers.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    static std::pair<float, float> NO_BINDING_POWER = {0, 0};
    std::unordered_map<Lexer::TokenType, std::pair<float, float>>
        BINDING_POWERS;

    void add_bp(Lexer::TokenType token_type,
                std::pair<float, float> binding_power)
    {
        BINDING_POWERS[token_type] = std::move(binding_power);
    }

    const std::pair<float, float> &get_bp(Lexer::TokenType token_type)
    {
        auto it = BINDING_POWERS.find(token_type);
        if (it == BINDING_POWERS.end())
            return NO_BINDING_POWER;

        return it->second;
    }

    std::unordered_map<Lexer::TokenType, NudHandler> NUD_HANDLER_MAP;

    void add_nud(Lexer::TokenType token_type, NudHandler handler,
                 std::optional<std::pair<float, float>> binding_power)
    {
        if (binding_power)
            add_bp(token_type, std::move(*binding_power));

        NUD_HANDLER_MAP[token_type] = std::move(handler);
    }

    NudHandler get_nud(Lexer::TokenType token_type)
    {
        auto it = NUD_HANDLER_MAP.find(token_type);
        if (it == NUD_HANDLER_MAP.end())
            return nullptr;

        return it->second;
    }

    std::unordered_map<Lexer::TokenType, LedHandler> LED_HANDLER_MAP;

    void add_led(Lexer::TokenType token_type, LedHandler handler,
                 std::optional<std::pair<float, float>> binding_power)
    {
        if (binding_power)
            add_bp(token_type, std::move(*binding_power));

        LED_HANDLER_MAP[token_type] = std::move(handler);
    }

    LedHandler get_led(Lexer::TokenType token_type)
    {
        auto it = LED_HANDLER_MAP.find(token_type);
        if (it == LED_HANDLER_MAP.end())
            return nullptr;

        return it->second;
    }

    ExprRule::ExprRule()
        : GrammarRule(Lexer::TokenTypes::Miscellaneous::EndOfFile)
    {
        initialize();
    }

    void ExprRule::initialize()
    {
        // No rules for expressions
        // Binding Power and Handlers are initialized here

        using namespace Lexer::TokenTypes;
        using namespace Operator;

        // Literal Expression
        float primary_bp = static_cast<int>(BindingPower::Primary);

        for (const auto &type : std::vector<Lexer::TokenType>(
                 {Primary::Int, Primary::Float, Primary::Boolean,
                  Primary::Character, Primary::String}))
        {
            add_nud(type, parse_literal,
                    std::pair<float, float>({primary_bp, primary_bp}));
        }

        // Identifier
        add_nud(Primary::Identifier, parse_identifier,
                std::pair<float, float>({primary_bp, primary_bp}));

        // Grouping
        add_nud(Delimeter::ParenthesisOpen,
                [&](Parser &parser,
                    DiagnosticList &diagnostics) -> std::unique_ptr<AST::Expr>
                {
                    std::unique_ptr<AST::Expr> expr =
                        parse_expr(parser, 0).node;

                    std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic =
                        parser.expect_or_error(
                            Lexer::TokenTypes::Delimeter::ParenthesisClose);

                    parser.lexer().next();

                    if (diagnostic != nullptr)
                    {
                        diagnostics.push_back(std::move(diagnostic));
                        return nullptr;
                    }

                    return expr;
                });

        // Unary
        float unary_bp = static_cast<int>(BindingPower::Unary);
        for (const auto &type : std::vector<Lexer::TokenType>({
                 Unary::Increment,
                 Unary::Decrement,
             }))
        {
            add_nud(
                type, [](Parser &parser, DiagnosticList &diagnostics)
                { return parse_unary(parser, diagnostics); },
                std::pair<float, float>({unary_bp, unary_bp}));

            add_led(
                type, [](Parser &parser, std::unique_ptr<AST::Expr> &left,
                         float right_bp, DiagnosticList &diagnostics)
                { return parse_unary(parser, left, right_bp, diagnostics); });
        }

        // Multiplicative
        float multiplicative_bp =
            static_cast<int>(BindingPower::Multiplicative);
        for (const auto &type : std::vector<Lexer::TokenType>({
                 Arithmetic::Multiplication,
                 Arithmetic::Division,
                 Arithmetic::Modulo,
             }))
        {
            add_led(type, parse_binary,
                    std::pair<float, float>(
                        {multiplicative_bp, multiplicative_bp}));
        }

        // Additive
        float additive_bp = static_cast<int>(BindingPower::Additive);
        for (const auto &type : std::vector<Lexer::TokenType>({
                 ArithmeticUnary::Plus,
                 ArithmeticUnary::Minus,
             }))
        {

            add_led(type, parse_binary,
                    std::pair<float, float>({additive_bp, additive_bp}));
        }

        // Relational
        float relational_bp = static_cast<int>(BindingPower::Relational);
        for (const auto &type : std::vector<Lexer::TokenType>({
                 Relational::Equal,
                 Relational::NotEqual,
                 Relational::LessThanEqual,
                 Relational::LessThan,
                 Relational::GreaterThanEqual,
                 Relational::GreaterThan,
             }))
        {
            add_led(type, parse_binary,
                    std::pair<float, float>({relational_bp, relational_bp}));
        }

        // Logical
        float logical_bp = static_cast<int>(BindingPower::Logical);
        for (const auto &type : std::vector<Lexer::TokenType>({
                 Logical::Not,
                 Logical::And,
             }))
        {
            add_led(type, parse_binary,
                    std::pair<float, float>({logical_bp, logical_bp}));
        }

        // Assignment
        float assignment_bp = static_cast<int>(BindingPower::Assignment);
        for (const auto &type : std::vector<Lexer::TokenType>({
                 Assignment::Direct,
                 Assignment::Addition,
                 Assignment::Subtraction,
                 Assignment::Multiplication,
                 Assignment::Division,
                 Assignment::Modulo,
             }))
        {
            add_led(type, parse_binary,
                    std::pair<float, float>({assignment_bp, assignment_bp}));
        }
    }

    ExprParseResult ExprRule::parse_expr(Parser &parser, float right_bp)
    {
        ExprParseResult result = {ParseResultStatus::Success, nullptr, {}};

        auto &lexer = parser.lexer();
        if (lexer.eof())
        {
            result.status = ParseResultStatus::Failed;
            return result;
        }

        Lexer::Token *token = lexer.peek();
        NudHandler nud = get_nud(token->type);

        if (nud == nullptr)
        {
            result.status = ParseResultStatus::Failed;
            result.diagnostics.push_back(
                Diagnostic::create_syntax_error(token));

            return result;
        }

        lexer.next();
        std::unique_ptr<AST::Expr> left = nud(parser, result.diagnostics);

        while (!lexer.eof())
        {
            token = lexer.peek();
            if (token == nullptr)
                break;

            auto &[left_bp_, right_bp_] = get_bp(token->type);
            if (right_bp > left_bp_)
                break;

            LedHandler led = get_led(token->type);
            if (led == nullptr)
            {
                // result.status = ParseResultStatus::Failed;
                // result.diagnostics.push_back(
                //     Diagnostic::create_syntax_error(token));

                break;
            }

            lexer.next();
            left = led(parser, left, right_bp_, result.diagnostics);
        }

        result.node = std::move(left);

        return result;
    }

    ParseResult ExprRule::parse(Parser &parser)
    {
        ExprParseResult expr_result = parse_expr(parser, 0);
        ParseResult result = {expr_result.status, std::move(expr_result.node),
                              std::move(expr_result.diagnostics)};

        if (result.status == ParseResultStatus::Failed)
        {
            result.node = nullptr;
            return result;
        }

        return result;
    }

} // namespace Parser
