#pragma once

#include <functional>
#include <unordered_map>
#include <utility>

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

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

    static std::pair<float, float> NO_BINDING_POWER = {0, 0};

    struct ExprParseResult : public ParseResult
    {
        std::unique_ptr<AST::Expr> data;

        ExprParseResult(Parser &parser, Core::ResultStatus status,
                        std::unique_ptr<AST::Expr> data,
                        Diagnostic::DiagnosticList diagnostics);
    };

    using NudHandler =
        std::function<std::unique_ptr<AST::Expr>(Parser &, ExprParseResult &)>;

    using LedHandler = std::function<std::unique_ptr<AST::Expr>(
        Parser &, std::unique_ptr<AST::Expr> &, float, ExprParseResult &)>;

    struct ExprHandler
    {
        Lexer::TokenType type;
        std::pair<float, float> bp;
        NudHandler nud;
        LedHandler led;
    };

    struct Expr : GrammarRule
    {
        Expr();

        void add_handler(Lexer::TokenType type, ExprHandler &&handler);
        ExprHandler *get_handler(Lexer::TokenType type);

        ExprParseResult parse_expr(Parser &parser, float right_bp = 0);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

      private:
        std::unordered_map<Lexer::TokenType, ExprHandler> handlers_;

        void initialize();
    };

} // namespace Parser
