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

    struct PrattParseResult : ParseResult
    {
        std::unique_ptr<AST::Node> data;

        PrattParseResult(Parser &parser, Core::ResultStatus status,
                         std::unique_ptr<AST::Expr> data,
                         Diagnostic::DiagnosticList diagnostics);
    };

    using NudHandler =
        std::function<std::unique_ptr<AST::Node>(Parser &, PrattParseResult &)>;

    using LedHandler = std::function<std::unique_ptr<AST::Expr>(
        Parser &, std::unique_ptr<AST::Node> &, float, PrattParseResult &)>;

    struct PrattHandler
    {
        Lexer::TokenType type;
        std::pair<float, float> bp;
        NudHandler nud;
        LedHandler led;
    };

    struct Pratt : GrammarRule
    {
        Pratt();

        void add_handler(Lexer::TokenType type, PrattHandler &&handler);
        PrattHandler *get_handler(Lexer::TokenType type);

        PrattParseResult parse_base(Parser &parser, float right_bp = 0);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

      private:
        std::unordered_map<Lexer::TokenType, PrattHandler> handlers_;

        void initialize();
    };

} // namespace Parser
