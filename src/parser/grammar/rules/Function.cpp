#include <iostream>

#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "ast/stmt/function/FunctionRetStmt.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Function.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/pointer.hpp"

namespace Parser
{
    using Lexer::TokenType;

    FunctionParameter::FunctionParameter()
        : GrammarRule(TokenType::EndOfFile, GC_LOCAL)
    {
    }

    ParseResult FunctionParameter::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void FunctionParameter::parse(Parser &parser, ParseResult &result)
    {
        // mut? IDENTIFIER ":" mut? TYPE ( "=" DEFAULT_VALUE )?

        auto &lexer = parser.lexer;

        // mut?
        AST::IdentifierMutabilityState mut_state =
            AST::IdentifierMutabilityState::Immutable;
        if (parser.expect(TokenType::Mut, false))
        {
            mut_state = AST::IdentifierMutabilityState::Mutable;
            lexer.consume();
        }

        // IDENTIFIER
        Lexer::Token *identifier = nullptr;
        if (auto diagnostic =
                parser.expect_or_error(TokenType::Identifier, false))
            result.error(std::move(diagnostic));
        else
            identifier = lexer.next();

        // : (TokenType::Colon)
        if (auto diagnostic = parser.expect_or_error(TokenType::Colon, false))
            result.error(std::move(diagnostic));
        else
            lexer.consume();

        // TYPE
        ParseResult t_res = Common::Pratt.parse_base(parser, 0, true);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        auto type = utils::dynamic_ptr_cast<AST::Type>(t_res.data);
        if (type == nullptr)
            return;

        // Optional Default Value
        // =
        std::unique_ptr<AST::Expr> value;
        if (parser.expect(TokenType::Equal, false))
        {
            lexer.consume();

            ParseResult v_res = Common::Pratt.parse_base(parser, 0);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            value = utils::dynamic_ptr_cast<AST::Expr>(v_res.data);
        }

        result.data = std::make_unique<AST::FunctionParameter>(
            *identifier, mut_state, std::move(type), std::move(value));
    }

    void FunctionParameter::recover(Parser &parser)
    {
        using Lexer::TokenType;

        parser.synchronize({TokenType::Comma, TokenType::RightParen});
    }

    FunctionDefinition::FunctionDefinition()
        : GrammarRule(Hyacinth::FUNCTION, GC_GLOBAL)
    {
    }

    std::vector<std::unique_ptr<AST::FunctionParameter>>
    FunctionDefinition ::parse_parameters(Parser &parser, ParseResult &result)
    {
        auto &lexer = parser.lexer;

        std::vector<std::unique_ptr<AST::FunctionParameter>> parameters;
        parameters.reserve(8);

        Lexer::TokenType closing = TokenType::RightParen;

        auto expect_param = true;
        ParseResult p_res{parser, Core::ResultStatus::Success, nullptr, {}};

        while (!parser.expect(closing, false))
        {
            if (expect_param)
            {
                param_rule.parse(parser, p_res);
                result.adapt(p_res.status, std::move(p_res.diagnostics));

                if (p_res.data == nullptr)
                    break;

                auto param =
                    utils::dynamic_ptr_cast<AST::FunctionParameter>(p_res.data);
                if (param == nullptr)
                    break;

                parameters.push_back(std::move(param));

                // Revert to initial state
                p_res.status = Core::ResultStatus::Success;

                expect_param = false;
            }

            if (parser.expect(TokenType::Comma, false))
            {
                lexer.consume();
                expect_param = true;

                continue;
            }

            break;
        }

        return parameters;
    }

    Core::Position *FunctionDefinition::parse_closing(Parser &parser,
                                                      ParseResult &result,
                                                      Lexer::TokenType expected)
    {
        auto &lexer = parser.lexer;

        if (auto diagnostic = parser.expect_or_error(expected, false))
        {
            // parser.panic();
            // result.status = Core::ResultStatus::Fail;
            // result.diagnostics.push_back(std::move(diagnostic));

            result.force_error(std::move(diagnostic));
            return nullptr;
        }

        return &lexer.next()->range.end;
    }

    ParseResult
    FunctionDefinition::parse(Parser &parser,
                              AST::DeclarationAccessibility accessibility)
    {
        ParseResult result = parse(parser);

        if (auto decl =
                dynamic_cast<AST::FunctionDeclarationStmt *>(result.data.get()))
            decl->accessibility = accessibility;

        return result;
    }

    ParseResult FunctionDefinition::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void FunctionDefinition::parse(Parser &parser, ParseResult &result)
    {
        // fn IDENTIFIER "->" RETURN_TYPE "(" PARAM_LIST ")" ( BLOCK | ";" )

        auto &lexer = parser.lexer;

        // fn
        if (auto diagnostic = parser.expect_or_error(token_type, false))
            result.force_error(std::move(diagnostic));
        else
            lexer.next();

        // IDENTIFIER
        Lexer::Token *identifier = nullptr;
        if (auto diagnostic =
                parser.expect_or_error(TokenType::Identifier, false))
            result.error(std::move(diagnostic));
        else
            identifier = lexer.next();

        // -> (TokenType::MinusGreater)
        if (auto diagnostic =
                parser.expect_or_error(TokenType::MinusGreater, false))
            result.error(std::move(diagnostic));
        else
            lexer.consume();

        // RETURN_TYPE
        ParseResult rt_res = Common::Pratt.parse_base(parser, 0, true);
        result.adapt(rt_res.status, std::move(rt_res.diagnostics));

        auto ret_type = utils::dynamic_ptr_cast<AST::Type>(rt_res.data);
        if (ret_type == nullptr)
            return;

        // ( (TokenType::LeftParen)
        if (auto diagnostic =
                parser.expect_or_error(TokenType::LeftParen, false))
            result.error(std::move(diagnostic));
        else
            lexer.consume();

        // PARAM_LIST
        std::vector<std::unique_ptr<AST::FunctionParameter>> parameters =
            parse_parameters(parser, result);

        // ) (TokenType::RightParen)
        Core::Position *param_cl =
            parse_closing(parser, result, TokenType::RightParen);

        // BLOCK
        // { (TokenType::LeftBrace)
        if (parser.expect(TokenType::LeftBrace, false))
        {
            ParseResult b_res = Common::Block.parse(parser);
            result.adapt(b_res.status, std::move(b_res.diagnostics));

            auto body = utils::dynamic_ptr_cast<AST::BlockStmt>(b_res.data);
            if (body == nullptr)
                return;

            result.data = std::make_unique<AST::FunctionDefinitionStmt>(
                *identifier, std::move(ret_type), std::move(parameters),
                std::move(body));
        }

        // ;
        else
        {
            ParseResult t_res = Common::Terminator.parse(parser);
            result.adapt(t_res.status, std::move(t_res.diagnostics));

            auto fn_node = std::make_unique<AST::FunctionDeclarationStmt>(
                *identifier, std::move(ret_type), std::move(parameters));

            fn_node->end_position = param_cl;
            result.data = std::move(fn_node);
        }
    }

    void FunctionDefinition::recover(Parser &parser)
    {
        using Lexer::TokenType;

        parser.synchronize({TokenType::LeftBrace, TokenType::RightBrace});
    }

    FunctionDefinitionReturn::FunctionDefinitionReturn()
        : GrammarRule(Lexer::TokenType::Return, GC_LOCAL)
    {
    }

    ParseResult FunctionDefinitionReturn::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void FunctionDefinitionReturn::parse(Parser &parser, ParseResult &result)
    {
        // return ( VALUE )? ";"

        auto &lexer = parser.lexer;

        // return
        Core::Position *ret_pos = nullptr;
        if (auto diagnostic = parser.expect_or_error(token_type, false))
            result.error(std::move(diagnostic));
        else
            ret_pos = &lexer.next()->range.start;

        std::unique_ptr<AST::Expr> value;

        // ; (TokenType::Semicolon)
        if (parser.expect(TokenType::Semicolon, false))
            Common::Terminator.parse(parser, result);
        else
        {
            // VALUE
            ParseResult v_res = Common::Pratt.parse_base(parser, 0);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            value = utils::dynamic_ptr_cast<AST::Expr>(v_res.data);

            // ; (TokenType::Semicolon)
            Common::Terminator.parse(parser, result);
        }

        result.data = std::make_unique<AST::FunctionReturnStmt>(
            *ret_pos, std::move(value));
    }

    void FunctionDefinitionReturn::recover(Parser &parser)
    {
        using Lexer::TokenType;

        parser.synchronize({TokenType::Semicolon});
    }

} // namespace Parser
