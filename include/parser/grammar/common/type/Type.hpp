#pragma once

#include <functional>
#include <optional>
#include <unordered_map>
#include <utility>

#include "ast/type/Type.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    enum class TypeBindingPower
    {
        Default = 0,
        Union,
        FunctionReturn,
        Separator,
        Generic,
        Primary,
    };

    extern std::unordered_map<Lexer::TokenType, std::pair<float, float>>
        TYPE_BINDING_POWERS;

    void add_type_bp(Lexer::TokenType token_type,
                     std::pair<float, float> type_bp);
    const std::pair<float, float> &get_type_bp(Lexer::TokenType token_type);

    struct TypeParseResult : ParseResult
    {
        std::unique_ptr<AST::Type> node;
    };

    using TypeNudHandler =
        std::function<std::unique_ptr<AST::Type>(Parser &, TypeParseResult &)>;
    extern std::unordered_map<Lexer::TokenType, TypeNudHandler>
        TYPE_NUD_HANDLER_MAP;

    void
    add_type_nud(Lexer::TokenType token_type, TypeNudHandler handler,
                 std::optional<std::pair<float, float>> type_bp = std::nullopt);
    TypeNudHandler get_type_nud(Lexer::TokenType token_type);

    using TypeLedHandler = std::function<std::unique_ptr<AST::Type>(
        Parser &, std::unique_ptr<AST::Type> &, float, TypeParseResult &)>;

    extern std::unordered_map<Lexer::TokenType, TypeLedHandler>
        TYPE_LED_HANDLER_MAP;

    void
    add_type_led(Lexer::TokenType token_type, TypeLedHandler handler,
                 std::optional<std::pair<float, float>> type_bp = std::nullopt);
    TypeLedHandler get_type_led(Lexer::TokenType token_type);

    class Type : public GrammarRule
    {
      public:
        Type();

        TypeParseResult parse_type(Parser &parser, float right_bp = 0);
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
