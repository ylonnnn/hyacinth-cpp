#include <unordered_map>
#include <utility>
#include <vector>

#include "parser/grammar/common/type/Type.hpp"
#include "parser/grammar/common/type/TypeHandlers.hpp"

namespace Parser
{
    static std::pair<float, float> NO_BINDING_POWER = {0, 0};
    std::unordered_map<Lexer::TokenType, std::pair<float, float>>
        TYPE_BINDING_POWERS;

    void add_type_bp(Lexer::TokenType token_type,
                     std::pair<float, float> type_bp)
    {
        TYPE_BINDING_POWERS.insert_or_assign(token_type, std::move(type_bp));
    }

    const std::pair<float, float> &get_type_bp(Lexer::TokenType token_type)
    {
        auto it = TYPE_BINDING_POWERS.find(token_type);
        if (it == TYPE_BINDING_POWERS.end())
            return NO_BINDING_POWER;

        return it->second;
    }

    std::unordered_map<Lexer::TokenType, TypeNudHandler> TYPE_NUD_HANDLER_MAP;

    void add_type_nud(Lexer::TokenType token_type, TypeNudHandler handler,
                      std::optional<std::pair<float, float>> type_bp)
    {
        if (type_bp)
            add_type_bp(token_type, std::move(*type_bp));

        TYPE_NUD_HANDLER_MAP.insert_or_assign(token_type, std::move(handler));
    }

    TypeNudHandler get_type_nud(Lexer::TokenType token_type)
    {
        auto it = TYPE_NUD_HANDLER_MAP.find(token_type);
        if (it == TYPE_NUD_HANDLER_MAP.end())
            return nullptr;

        return it->second;
    }

    std::unordered_map<Lexer::TokenType, TypeLedHandler> TYPE_LED_HANDLER_MAP;

    void add_type_led(Lexer::TokenType token_type, TypeLedHandler handler,
                      std::optional<std::pair<float, float>> type_bp)
    {
        if (type_bp)
            add_type_bp(token_type, std::move(*type_bp));

        TYPE_LED_HANDLER_MAP.insert_or_assign(token_type, std::move(handler));
    }

    TypeLedHandler get_type_led(Lexer::TokenType token_type)
    {
        auto it = TYPE_LED_HANDLER_MAP.find(token_type);
        if (it == TYPE_LED_HANDLER_MAP.end())
            return nullptr;

        return it->second;
    }

    static void initialize()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        TYPE_BINDING_POWERS.reserve(32);
        TYPE_NUD_HANDLER_MAP.reserve(32);
        TYPE_LED_HANDLER_MAP.reserve(32);

        float primary_bp = static_cast<int>(TypeBindingPower::Primary);
        add_type_nud(Primary::Identifier, parse_simple,
                     std::pair<float, float>{primary_bp, primary_bp});

        for (const auto &type : std::vector<Lexer::TokenType>{
                 Primary::Int, Primary::Float, Primary::Boolean,
                 Primary::Character, Primary::String})
            add_type_nud(type, parse_constant,
                         std::pair<float, float>{primary_bp, primary_bp});

        float generic_bp = static_cast<int>(TypeBindingPower::Generic);
        add_type_led(Relational::LessThan, parse_generic,
                     std::pair<float, float>{generic_bp, generic_bp});
    }

    static bool initialized = (initialize(), true);

    Type::Type() : GrammarRule(Lexer::TokenTypes::Miscellaneous::EndOfFile) {}

    TypeParseResult::TypeParseResult(Parser &parser, Core::ResultStatus status,
                                     std::unique_ptr<AST::Type> data,
                                     Diagnostic::DiagnosticList diagnostics)
        : ParseResult(parser, status, std::move(data), std::move(diagnostics))
    {
        std::unique_ptr<AST::Node> node = std::move(ParseResult::data);
        if (dynamic_cast<AST::Type *>(node.get()))
            this->data = std::unique_ptr<AST::Type>(
                static_cast<AST::Type *>(node.release()));
    }

    TypeParseResult Type::parse_type(Parser &parser, float right_bp)
    {
        TypeParseResult result = {
            parser, Core::ResultStatus::Success, nullptr, {}};

        auto &lexer = parser.lexer();
        if (lexer.eof())
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        Lexer::Token *token = lexer.peek();
        TypeNudHandler nud = get_type_nud(token->type);

        if (nud == nullptr)
        {
            result.error(Diagnostic::create_syntax_error(token));
            return result;
        }

        lexer.next();
        std::unique_ptr<AST::Type> left = nud(parser, result);

        while (!lexer.eof())
        {
            token = lexer.peek();
            if (token == nullptr)
                break;

            auto &[left_bp_, right_bp_] = get_type_bp(token->type);
            if (right_bp > left_bp_)
                break;

            TypeLedHandler led = get_type_led(token->type);
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

    ParseResult Type::parse(Parser &parser)
    {
        TypeParseResult type_result = parse_type(parser, 0);
        ParseResult result = {parser, type_result.status,
                              std::move(type_result.data),
                              std::move(type_result.diagnostics)};

        if (result.status == Core::ResultStatus::Fail)
            result.data = nullptr;

        return result;
    }
} // namespace Parser
