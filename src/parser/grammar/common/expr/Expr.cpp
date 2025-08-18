#include <unordered_map>
#include <utility>

#include "ast/stmt/ExprStmt.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/expr/Expr.hpp"
#include "parser/grammar/common/expr/ExprHandlers.hpp"

namespace Parser
{
    static std::pair<float, float> NO_BINDING_POWER = {0, 0};
    std::unordered_map<Lexer::TokenType, std::pair<float, float>>
        BINDING_POWERS;

    void add_bp(Lexer::TokenType token_type,
                std::pair<float, float> binding_power)
    {
        if (!BINDING_POWERS.bucket_count())
            BINDING_POWERS.reserve(32);

        BINDING_POWERS[token_type] = std::move(binding_power);
        // BINDING_POWERS.insert_or_assign(token_type,
        // std::move(binding_power));
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

        if (!NUD_HANDLER_MAP.bucket_count())
            NUD_HANDLER_MAP.reserve(32);

        NUD_HANDLER_MAP.insert_or_assign(token_type, std::move(handler));
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

        if (!LED_HANDLER_MAP.bucket_count())
            LED_HANDLER_MAP.reserve(32);

        LED_HANDLER_MAP.insert_or_assign(token_type, std::move(handler));
    }

    LedHandler get_led(Lexer::TokenType token_type)
    {
        auto it = LED_HANDLER_MAP.find(token_type);
        if (it == LED_HANDLER_MAP.end())
            return nullptr;

        return it->second;
    }

    static void initialize()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        // Literal Expression
        float primary_bp = static_cast<int>(BindingPower::Primary);

        for (const auto &type : std::vector<Lexer::TokenType>(
                 {Primary::Int, Primary::Float, Primary::Boolean,
                  Primary::Character, Primary::String}))
        {
            add_nud(type, parse_literal,
                    std::pair<float, float>{primary_bp, primary_bp});
        }

        // Identifier
        add_nud(Primary::Identifier, parse_idtype_expr,
                std::pair<float, float>{primary_bp, primary_bp});

        // Grouping
        add_nud(Delimeter::ParenthesisOpen,
                [&](Parser &parser,
                    ExprParseResult &result) -> std::unique_ptr<AST::Expr>
                {
                    Expr &expr_rule = Common::Expr;

                    ExprParseResult expr_res = expr_rule.parse_expr(parser, 0);
                    std::unique_ptr<AST::Expr> expr = std::move(expr_res.data);

                    result.adapt(expr_res.status,
                                 std::move(expr_res.diagnostics));

                    if (auto diagnostic = parser.expect_or_error(
                            Delimeter::ParenthesisClose, false))
                    {
                        result.error(std::move(diagnostic));

                        return nullptr;
                    }
                    else
                        parser.lexer().next();

                    return expr;
                });

        // Compound
        add_nud(Delimeter::BraceOpen, parse_array);
        add_led(Delimeter::BraceOpen, parse_instance);

        // Member Access
        float memaccess_bp = static_cast<int>(BindingPower::MemberAccess);
        for (const auto &type :
             std::vector<Lexer::TokenType>{Operator::Dot::Single})
        {
            add_led(type, parse_memaccess,
                    std::pair<float, float>{memaccess_bp, memaccess_bp});
        }

        // Function Call
        float fncall_bp = static_cast<int>(BindingPower::FunctionCall);
        for (const auto &type :
             std::vector<Lexer::TokenType>{Delimeter::ParenthesisOpen})
        {
            add_led(type, parse_fncall,
                    std::pair<float, float>{fncall_bp, fncall_bp});
        }

        // Unary
        float unary_bp = static_cast<int>(BindingPower::Unary);
        for (const auto &type : std::vector<Lexer::TokenType>({
                 Unary::Increment,
                 Unary::Decrement,
             }))
        {
            add_nud(
                type, [](Parser &parser, ExprParseResult &result)
                { return parse_unary(parser, result); },
                std::pair<float, float>{unary_bp, unary_bp});

            add_led(type, [](Parser &parser, std::unique_ptr<AST::Expr> &left,
                             float right_bp, ExprParseResult &result)
                    { return parse_unary(parser, left, right_bp, result); });
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
                    std::pair<float, float>{additive_bp, additive_bp});
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
                    std::pair<float, float>{relational_bp, relational_bp});
        }

        // Logical
        float logical_bp = static_cast<int>(BindingPower::Logical);
        for (const auto &type : std::vector<Lexer::TokenType>({
                 Logical::Not,
                 Logical::And,
             }))
        {
            add_led(type, parse_binary,
                    std::pair<float, float>{logical_bp, logical_bp});
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
                    std::pair<float, float>{assignment_bp, assignment_bp});
        }
    }

    static bool initialized = (initialize(), true);

    Expr::Expr() : GrammarRule(Lexer::TokenTypes::Miscellaneous::EndOfFile) {}

    ExprParseResult::ExprParseResult(Parser &parser, Core::ResultStatus status,
                                     std::unique_ptr<AST::Expr> data,
                                     Diagnostic::DiagnosticList diagnostics)
        : ParseResult(parser, status, std::move(data), std::move(diagnostics))
    {
        std::unique_ptr<AST::Node> node = std::move(ParseResult::data);
        if (auto ptr = dynamic_cast<AST::Expr *>(node.release()))
            this->data = std::unique_ptr<AST::Expr>(ptr);
    }

    ExprParseResult Expr::parse_expr(Parser &parser, float right_bp)
    {
        ExprParseResult result = {
            parser, Core::ResultStatus::Success, nullptr, {}};

        auto &lexer = parser.lexer();
        if (lexer.eof())
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        Lexer::Token *token = lexer.peek();
        NudHandler nud = get_nud(token->type);

        if (nud == nullptr)
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        lexer.next();
        std::unique_ptr<AST::Expr> left = nud(parser, result);

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
                // result.status = Core::ResultStatus::Fail;
                // result.diagnostics.push_back(
                //     Diagnostic::create_syntax_error(token));

                break;
            }

            lexer.next();
            left = led(parser, left, right_bp_, result);
        }

        result.data = std::move(left);

        return result;
    }

    ParseResult Expr::parse(Parser &parser)
    {
        ExprParseResult expr_result = parse_expr(parser, 0);

        if (expr_result.data == nullptr)
            return {parser, expr_result.status, nullptr,
                    std::move(expr_result.diagnostics)};

        ParseResult result = {
            parser, expr_result.status,
            std::make_unique<AST::ExprStmt>(std::move(expr_result.data)),
            std::move(expr_result.diagnostics)};

        if (result.status == Core::ResultStatus::Fail)
        {
            result.data = nullptr;
            return result;
        }

        ParseResult t_res = Common::Terminator.parse(parser);

        if (t_res.status == Core::ResultStatus::Fail)
        {
            result.status = t_res.status;
            result.data = nullptr;
        }

        result.adapt(std::move(t_res.diagnostics));

        return result;
    }

} // namespace Parser
