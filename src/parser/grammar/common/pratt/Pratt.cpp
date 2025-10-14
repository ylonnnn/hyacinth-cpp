#include <unordered_map>
#include <utility>

// #include "ast/stmt/ExprStmt.hpp"
#include "diagnostic/helpers.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarContext.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/pratt/Pratt.hpp"
#include "parser/grammar/common/pratt/PrattHandlers.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    Pratt::Pratt() : GrammarRule(Lexer::TokenType::EndOfFile, GC_LOCAL)
    {
        initialize();
        initialize_types();
    }

    void Pratt::initialize()
    {
        using TokenType = Lexer::TokenType;

        float default_bp = static_cast<int32_t>(BindingPower::Default);

        // Literal
        float primary_bp = static_cast<int32_t>(BindingPower::Primary);

        for (const auto &type : std::vector<TokenType>(
                 {TokenType::Int, TokenType::Float, TokenType::Bool,
                  TokenType::Char, TokenType::String}))
        {
            add_handler(type, PrattHandler{.type = type,
                                           .bp = {primary_bp, primary_bp},
                                           .nud = parse_literal,
                                           .led = nullptr});
        }

        // Identifier
        add_handler(
            TokenType::Identifier,
            PrattHandler{
                .type = TokenType::Identifier,
                .bp = {primary_bp, primary_bp},
                .nud = [&](Parser &parser,
                           ParseResult &result) -> std::unique_ptr<AST::Path>
                {
                    std::vector<std::unique_ptr<AST::Identifier>> segments;
                    segments.push_back(
                        std::move(parse_identifier(parser, result)));

                    return std::make_unique<AST::Path>(std::move(segments));
                },
                .led = nullptr,
            });

        // Grouping & Function Call
        float func_call_bp = static_cast<int32_t>(BindingPower::FunctionCall);
        add_handler(
            TokenType::LeftParen,
            PrattHandler{
                .type = TokenType::LeftParen,
                .bp = {func_call_bp, default_bp},
                .nud = make_group_handler(
                    TokenType::RightParen, [&](Parser &parser) -> ParseResult
                    { return Common::Pratt.parse_base(parser, 0); }),
                .led = parse_func_call,
            });

        // // Compound
        // add_nud(Delimeter::BraceOpen, parse_array);
        // add_led(Delimeter::BraceOpen, parse_instance);

        // // Element Access
        // add_led(Delimeter::BracketOpen, parse_elaccess);

        // Member Access
        float memaccess_bp = static_cast<int32_t>(BindingPower::MemberAccess);

        for (const auto &type : std::vector<TokenType>({
                 TokenType::Dot,
                 TokenType::QuestionDot,
             }))
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {memaccess_bp, memaccess_bp + 1.0f},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // Path
        add_handler(Hyacinth::PATH_SEP, PrattHandler{
                                            .type = Hyacinth::PATH_SEP,
                                            .bp = {memaccess_bp, memaccess_bp},
                                            .nud = nullptr,
                                            .led = parse_path,
                                        });

        // Unary
        float unary_bp = static_cast<int32_t>(BindingPower::Unary);

        // Pre
        for (const auto &type : std::vector<TokenType>{
                 TokenType::Bang,
                 TokenType::Tilde,
             })
        {
            add_handler(type,
                        PrattHandler{
                            .type = type,
                            .bp = {unary_bp, unary_bp},
                            .nud = [&](Parser &parser, ParseResult &result)
                                -> std::unique_ptr<AST::UnaryExpr>
                            { return parse_unary(parser, result); },
                            .led = nullptr,
                        });
        }

        // Pre & Post
        for (const auto &type : std::vector<TokenType>{
                 TokenType::PlusPlus,
                 TokenType::MinusMinus,
             })
        {
            add_handler(
                type,
                PrattHandler{
                    .type = type,
                    .bp = {unary_bp, unary_bp},
                    .nud = [&](Parser &parser, ParseResult &result)
                        -> std::unique_ptr<AST::UnaryExpr>
                    { return parse_unary(parser, result); },
                    .led = [&](Parser &parser, std::unique_ptr<AST::Node> &left,
                               float right_bp, ParseResult &result)
                        -> std::unique_ptr<AST::UnaryExpr>
                    { return parse_unary(parser, left, right_bp, result); },
                });
        }

        // Exponentiation
        float exponentiation_bp =
            static_cast<int32_t>(BindingPower::Exponentiation);
        add_handler(TokenType::CaretCaret,
                    PrattHandler{
                        .type = TokenType::CaretCaret,
                        .bp = {exponentiation_bp, exponentiation_bp},
                        .nud = nullptr,
                        .led = parse_binary,
                    });

        // Multiplicative
        float multiplicative_bp =
            static_cast<int32_t>(BindingPower::Multiplicative);
        for (const auto &type : std::vector<TokenType>(
                 {TokenType::Star, TokenType::Slash, TokenType::Percent}))
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {multiplicative_bp, multiplicative_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // Additive
        float additive_bp = static_cast<int32_t>(BindingPower::Additive);
        for (const auto &type : std::vector<TokenType>({
                 TokenType::Plus,
                 TokenType::Minus,
             }))
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {additive_bp, additive_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // BitwiseShift
        float bitwise_shift_bp =
            static_cast<int32_t>(BindingPower::BitwiseShift);
        for (const auto &type : std::vector<TokenType>{
                 TokenType::LessLess, TokenType::GreaterGreater})
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {bitwise_shift_bp, bitwise_shift_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // Relational
        float relational_bp = static_cast<int32_t>(BindingPower::Relational);
        for (const auto &type : std::vector<TokenType>({
                 TokenType::Equal,
                 TokenType::BangEqual,
                 TokenType::LessEqual,
                 TokenType::Less,
                 TokenType::GreaterEqual,
                 TokenType::Greater,
             }))
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {relational_bp, relational_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // Bitwise
        float bitwise_bp = static_cast<int32_t>(BindingPower::Bitwise);
        for (const auto &type :
             std::vector<TokenType>{TokenType::Pipe, TokenType::Caret})
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {bitwise_bp, bitwise_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // Bitwise & Address-Of
        add_handler(TokenType::Ampersand,
                    PrattHandler{
                        .type = TokenType::Ampersand,
                        .bp = {bitwise_bp + 1.0f, unary_bp},
                        .nud = [&](Parser &parser, ParseResult &result)
                            -> std::unique_ptr<AST::UnaryExpr>
                        { return parse_unary(parser, result); },
                        .led = parse_binary,
                    });

        // ConditionalLogical
        float cond_logical_bp =
            static_cast<int32_t>(BindingPower::ConditionalLogical);
        for (const auto &type : std::vector<TokenType>({
                 TokenType::Bang,
                 TokenType::AmpersandAmpersand,
                 TokenType::PipePipe,
             }))
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {cond_logical_bp, cond_logical_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // ConditionalSelection
        float cond_selection_bp =
            static_cast<int32_t>(BindingPower::ConditionalSelection);
        for (const auto &type :
             std::vector<TokenType>{TokenType::QuestionQuestion})
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {cond_selection_bp, cond_selection_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // Assignment
        float assignment_bp = static_cast<int32_t>(BindingPower::Assignment);
        for (const auto &type : std::vector<TokenType>{
                 TokenType::Equal,
                 TokenType::PlusEqual,
                 TokenType::MinusEqual,
                 TokenType::StarEqual,
                 TokenType::SlashEqual,
                 TokenType::PercentEqual,
             })
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = {assignment_bp, assignment_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }
    }

    void Pratt::initialize_types()
    {
        using TokenType = Lexer::TokenType;

        float default_tbp = static_cast<int32_t>(TypeBindingPower::Default);

        // Primary
        float primary_tbp = static_cast<int32_t>(TypeBindingPower::Primary);
        add_type_handler(TokenType::Identifier,
                         PrattHandler{
                             .type = TokenType::Identifier,
                             .bp = {primary_tbp, primary_tbp},
                             .nud = parse_type_identifier,
                             .led = nullptr,
                         });

        // Grouping
        add_type_handler(
            TokenType::LeftParen,
            PrattHandler{
                .type = TokenType::LeftParen,
                .bp = {default_tbp, default_tbp},
                .nud = make_group_handler(
                    TokenType::RightParen, [&](Parser &parser) -> ParseResult
                    { return Common::Pratt.parse_base(parser, 0, true); }),
                .led = nullptr,
            });

        // Array
        float array_tbp = static_cast<int32_t>(TypeBindingPower::Array);
        add_type_handler(TokenType::LeftBracket,
                         PrattHandler{
                             .type = TokenType::LeftBracket,
                             .bp = {array_tbp, array_tbp},
                             .nud = parse_type_array,
                             .led = nullptr,
                         });

        // Pointer
        float pointer_tbp = static_cast<int32_t>(TypeBindingPower::Pointer);
        add_type_handler(
            TokenType::Star,
            PrattHandler{
                .type = TokenType::Star,
                .bp = {pointer_tbp, pointer_tbp},
                .nud = make_type_pref_nud_handler(
                    TypeBindingPower::Pointer, AST::PrefixedTypeKind::Pointer),
                .led = nullptr,
            });

        // Reference
        float reference_tbp = static_cast<int32_t>(TypeBindingPower::Reference);
        add_type_handler(TokenType::Ampersand,
                         PrattHandler{
                             .type = TokenType::Ampersand,
                             .bp = {reference_tbp, reference_tbp},
                             .nud = make_type_pref_nud_handler(
                                 TypeBindingPower::Reference,
                                 AST::PrefixedTypeKind::Reference),
                             .led = nullptr,
                         });

        // RValue Reference
        add_type_handler(TokenType::AmpersandAmpersand,
                         PrattHandler{
                             .type = TokenType::AmpersandAmpersand,
                             .bp = {reference_tbp, reference_tbp},
                             .nud = make_type_pref_nud_handler(
                                 TypeBindingPower::Reference,
                                 AST::PrefixedTypeKind::Reference),
                             .led = nullptr,
                         });

        // Optional
        float optional_tbp = static_cast<int32_t>(TypeBindingPower::Optional);
        add_type_handler(TokenType::Question,
                         PrattHandler{
                             .type = TokenType::Question,
                             .bp = {optional_tbp, optional_tbp},
                             .nud = nullptr,
                             .led = make_type_suf_led_handler(
                                 TypeBindingPower::Optional,
                                 AST::SuffixedTypeKind::Optional),
                         });

        // Mutability Binding
        add_type_handler(TokenType::Mut, PrattHandler{
                                             .type = TokenType::Mut,
                                             .bp = {default_tbp, default_tbp},
                                             .nud = make_type_mod_nud_handler(
                                                 TypeBindingPower::Default,
                                                 AST::ModifierType::Mutable),
                                             .led = nullptr,
                                         });
    }

    void Pratt::add_handler(Lexer::TokenType type, PrattHandler &&handler)
    {
        handlers_.insert_or_assign(type, std::move(handler));
    }

    PrattHandler *Pratt::get_handler(Lexer::TokenType type)
    {
        auto it = handlers_.find(type);
        if (it == handlers_.end())
            return nullptr;

        return &it->second;
    }

    void Pratt::add_type_handler(Lexer::TokenType type, PrattHandler &&handler)
    {
        type_handlers_.insert_or_assign(type, std::move(handler));
    }

    PrattHandler *Pratt::get_type_handler(Lexer::TokenType type)
    {
        auto it = type_handlers_.find(type);
        if (it == type_handlers_.end())
            return nullptr;

        return &it->second;
    }

    ParseResult Pratt::parse_base(Parser &parser, float right_bp, bool type)
    {
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        auto &lexer = parser.lexer;
        if (lexer.eof())
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        Lexer::Token *token = lexer.peek();
        PrattHandler *handler =
            type ? get_type_handler(token->type) : get_handler(token->type);

        if (handler == nullptr || handler->nud == nullptr)
        {
            result.error(Diagnostic::create_syntax_error(*token));
            return result;
        }

        // lexer.consume();
        std::unique_ptr<AST::Node> left = handler->nud(parser, result);

        while (!lexer.eof())
        {
            token = lexer.peek();
            if (token == nullptr)
                break;

            PrattHandler *n_handler =
                type ? get_type_handler(token->type) : get_handler(token->type);

            if (n_handler == nullptr)
                break;

            auto &[_, bp, __, led] = *n_handler;
            auto &[left_bp_, right_bp_] = bp;

            if (right_bp > left_bp_)
                break;

            // result.error(Diagnostic::create_syntax_error(*token));
            if (led == nullptr)
                break;

            // lexer.consume();
            left = led(parser, left, right_bp_, result);
        }

        result.data = std::move(left);

        return result;
    }

    ParseResult Pratt::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void Pratt::parse(Parser &parser, ParseResult &result)
    {
        ParseResult expr_result = parse_base(parser, 0);

        if (expr_result.data == nullptr)
        {
            result.adapt(expr_result.status, std::move(expr_result.diagnostics),
                         nullptr);

            return;
        }

        result.adapt(expr_result.status, std::move(expr_result.diagnostics),
                     std::move(expr_result.data));

        if (result.status == Core::ResultStatus::Fail)
        {
            result.data = nullptr;
            return;
        }

        // ; (TERMINATOR)
        Common::Terminator.parse(parser, result);
    }

} // namespace Parser
