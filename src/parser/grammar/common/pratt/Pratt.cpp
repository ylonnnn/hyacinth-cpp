#include <unordered_map>
#include <utility>

// #include "ast/stmt/ExprStmt.hpp"
#include "ast/stmt/ExprStmt.hpp"
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
    PrattParseResult::PrattParseResult(Parser &parser,
                                       Core::ResultStatus status,
                                       std::unique_ptr<AST::Expr> data,
                                       Diagnostic::DiagnosticList diagnostics)
        : ParseResult(parser, status, std::move(data), std::move(diagnostics))
    {
        std::unique_ptr<AST::Node> node = std::move(ParseResult::data);
        if (auto ptr = dynamic_cast<AST::Expr *>(node.release()))
            this->data = std::unique_ptr<AST::Expr>(ptr);
    }

    Pratt::Pratt() : GrammarRule(Lexer::TokenType::EndOfFile, GC_LOCAL)
    {
        initialize();
    }

    void Pratt::initialize()
    {
        using namespace Lexer;

        // Literal Expression
        float primary_bp = static_cast<int>(BindingPower::Primary);

        for (const auto &type : std::vector<TokenType>(
                 {TokenType::Int, TokenType::Float, TokenType::Bool,
                  TokenType::Char, TokenType::String}))
        {
            add_handler(type,
                        PrattHandler{.type = type,
                                     .bp = std::pair<float, float>{primary_bp,
                                                                   primary_bp},
                                     .nud = parse_literal,
                                     .led = nullptr});
        }

        // Identifier
        add_handler(
            TokenType::Identifier,
            PrattHandler{
                .type = TokenType::Identifier,
                .bp = std::pair<float, float>{primary_bp, primary_bp},
                .nud = [&](Parser &parser, PrattParseResult &result)
                    -> std::unique_ptr<AST::Path>
                {
                    std::vector<std::unique_ptr<AST::Identifier>> segments;
                    segments.push_back(
                        std::move(parse_identifier(parser, result)));

                    return std::make_unique<AST::Path>(std::move(segments));
                },
                .led = nullptr,
            });

        // Grouping
        // add_nud(Delimeter::ParenthesisOpen,
        //         [&](Parser &parser,
        //             PrattParseResult &result) -> std::unique_ptr<AST::Expr>
        //         {
        //             Pratt &expr_rule = Common::Pratt;

        //             PrattParseResult expr_res = expr_rule.parse_expr(parser,
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

        // Member Access
        float memaccess_bp = static_cast<int>(BindingPower::MemberAccess);

        // Path
        add_handler(
            Hyacinth::PATH_SEP,
            PrattHandler{
                .type = Hyacinth::PATH_SEP,
                .bp = std::pair<float, float>{memaccess_bp, memaccess_bp},
                .nud = nullptr,
                .led = parse_path,
            });

        // // Function Call
        // float fncall_bp = static_cast<int>(BindingPower::FunctionCall);
        // for (const auto &type :
        //      std::vector<Lexer::TokenType>{Delimeter::ParenthesisOpen})
        // {
        //     add_led(type, parse_fncall,
        //             std::pair<float, float>{fncall_bp, fncall_bp});
        // }

        // Unary
        float unary_bp = static_cast<int>(BindingPower::Unary);
        for (const auto &type : std::vector<TokenType>({
                 TokenType::PlusPlus,
                 TokenType::MinusMinus,
                 TokenType::Bang,
                 TokenType::Tilde,
             }))
        {
            add_handler(
                type,
                PrattHandler{
                    .type = type,
                    .bp = std::pair<float, float>{unary_bp, unary_bp},
                    .nud = [&](Parser &parser, PrattParseResult &result)
                        -> std::unique_ptr<AST::UnaryExpr>
                    { return parse_unary(parser, result); },
                    .led = [&](Parser &parser, std::unique_ptr<AST::Node> &left,
                               float right_bp, PrattParseResult &result)
                        -> std::unique_ptr<AST::UnaryExpr>
                    { return parse_unary(parser, left, right_bp, result); },
                });
        }

        // Multiplicative
        float multiplicative_bp =
            static_cast<int>(BindingPower::Multiplicative);
        for (const auto &type : std::vector<TokenType>(
                 {TokenType::Star, TokenType::Slash, TokenType::Percent}))
        {
            add_handler(type,
                        PrattHandler{
                            .type = type,
                            .bp = std::pair<float, float>{multiplicative_bp,
                                                          multiplicative_bp},
                            .nud = nullptr,
                            .led = parse_binary,
                        });
        }

        // Additive
        float additive_bp = static_cast<int>(BindingPower::Additive);
        for (const auto &type : std::vector<TokenType>({
                 TokenType::Plus,
                 TokenType::Minus,
             }))
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = std::pair<float, float>{additive_bp,
                                                                additive_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // Relational
        float relational_bp = static_cast<int>(BindingPower::Relational);
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
                                  .bp = std::pair<float, float>{relational_bp,
                                                                relational_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }

        // Logical
        float logical_bp = static_cast<int>(BindingPower::Logical);
        for (const auto &type : std::vector<TokenType>({
                 TokenType::Bang,
                 TokenType::AmpersandAmpersand,
                 TokenType::PipePipe,
             }))
        {
            add_handler(
                type, PrattHandler{
                          .type = type,
                          .bp = std::pair<float, float>{logical_bp, logical_bp},
                          .nud = nullptr,
                          .led = parse_binary,
                      });
        }

        // Assignment
        float assignment_bp = static_cast<int>(BindingPower::Assignment);
        for (const auto &type : std::vector<TokenType>({
                 TokenType::Equal,
                 TokenType::PlusEqual,
                 TokenType::MinusEqual,
                 TokenType::StarEqual,
                 TokenType::SlashEqual,
                 TokenType::PercentEqual,
             }))
        {
            add_handler(type, PrattHandler{
                                  .type = type,
                                  .bp = std::pair<float, float>{assignment_bp,
                                                                assignment_bp},
                                  .nud = nullptr,
                                  .led = parse_binary,
                              });
        }
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

    PrattParseResult Pratt::parse_base(Parser &parser, float right_bp)
    {
        PrattParseResult result = {
            parser, Core::ResultStatus::Success, nullptr, {}};

        auto &lexer = parser.lexer;
        if (lexer.eof())
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        Lexer::Token *token = lexer.peek();
        PrattHandler *handler = get_handler(token->type);

        if (handler == nullptr || handler->nud == nullptr)
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        // lexer.consume();
        std::unique_ptr<AST::Node> left = handler->nud(parser, result);

        while (!lexer.eof())
        {
            token = lexer.peek();
            if (token == nullptr)
                break;

            PrattHandler *n_handler = get_handler(token->type);
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
        PrattParseResult expr_result = parse_base(parser, 0);

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
