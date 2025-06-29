#pragma once

#include <functional>
#include <optional>
#include <unordered_map>
#include <utility>

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    enum class BindingPower
    {
        Default = 0,
        Comma,
        Assignment,
        Logical,
        Relational,
        Additive,
        Multiplicative,
        Unary,
        FunctionCall,
        MemberAccess,
        Primary,
    };

    extern std::unordered_map<Lexer::TokenType, std::pair<float, float>>
        BINDING_POWERS;

    void add_bp(Lexer::TokenType token_type, std::pair<float, float>);
    const std::pair<float, float> &get_bp(Lexer::TokenType token_type);

    using NudHandler =
        std::function<std::unique_ptr<AST::Expr>(Parser &, DiagnosticList &)>;
    extern std::unordered_map<Lexer::TokenType, NudHandler> NUD_HANDLER_MAP;

    void add_nud(
        Lexer::TokenType token_type, NudHandler handler,
        std::optional<std::pair<float, float>> binding_power = std::nullopt);
    NudHandler get_nud(Lexer::TokenType token_type);

    using LedHandler = std::function<std::unique_ptr<AST::Expr>(
        Parser &, std::unique_ptr<AST::Expr> &, float, DiagnosticList &)>;
    // using LedHandler = std::function<std::unique_ptr<AST::Expr>(
    //     Parser &parser, std::unique_ptr<AST::Expr> &left, float right_bp)>;

    extern std::unordered_map<Lexer::TokenType, LedHandler> LED_HANDLER_MAP;

    void add_led(
        Lexer::TokenType token_type, LedHandler handler,
        std::optional<std::pair<float, float>> binding_power = std::nullopt);
    LedHandler get_led(Lexer::TokenType token_type);

    struct ExprParseResult : ParseResult
    {
        std::unique_ptr<AST::Expr> node;
    };

    class ExprRule : public GrammarRule
    {
      public:
        ExprRule();

        void initialize();

        ExprParseResult parse_expr(Parser &parser, float right_bp = 0);
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
