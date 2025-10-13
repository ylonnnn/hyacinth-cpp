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

    enum class TypeBindingPower
    {
        Default = 0,
        Optional,
        Reference,
        Pointer,
        Array,
        Primary,
    };

    static std::pair<float, float> NO_BINDING_POWER = {0, 0};

    template <typename T = AST::Node,
              typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
    using NudHandler =
        std::function<std::unique_ptr<T>(Parser &, ParseResult &)>;

    template <typename T = AST::Node,
              typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
    using LedHandler = std::function<std::unique_ptr<T>(
        Parser &, std::unique_ptr<AST::Node> &, float, ParseResult &)>;

    struct PrattHandler
    {
        Lexer::TokenType type;
        std::pair<float, float> bp;
        NudHandler<> nud;
        LedHandler<> led;
    };

    struct Pratt : GrammarRule
    {
        Pratt();

        void add_handler(Lexer::TokenType type, PrattHandler &&handler);
        PrattHandler *get_handler(Lexer::TokenType type);

        void add_type_handler(Lexer::TokenType type, PrattHandler &&handler);
        PrattHandler *get_type_handler(Lexer::TokenType type);

        ParseResult parse_base(Parser &parser, float right_bp = 0,
                               bool type = false);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;

      private:
        std::unordered_map<Lexer::TokenType, PrattHandler> handlers_;
        std::unordered_map<Lexer::TokenType, PrattHandler> type_handlers_;

        void initialize();
        void initialize_types();
    };

} // namespace Parser
