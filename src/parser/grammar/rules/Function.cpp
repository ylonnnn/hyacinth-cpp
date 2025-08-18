#include <iostream>

#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "ast/stmt/function/FunctionRetStmt.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/IdentifierInit.hpp"
#include "parser/grammar/rules/Function.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/style.hpp"

namespace Parser
{
    using namespace Lexer::TokenTypes;

    FunctionDefinition::FunctionDefinition() : GrammarRule(Hyacinth::FUNCTION)
    {
    }

    static std::vector<AST::FunctionParameter>
    parse_parameters(Parser &parser, ParseResult &result)
    {
        auto &lexer = parser.lexer();

        std::vector<AST::FunctionParameter> parameters;
        parameters.reserve(8);

        Lexer::TokenType closing = Delimeter::ParenthesisClose;

        auto &identifier = Common::IdentifierInitialization;
        auto expect_param = true;

        while (!parser.expect(closing, false))
        {
            if (expect_param)
            {
                ParseResult i_res = identifier.parse(parser);
                result.adapt(i_res.status, std::move(i_res.diagnostics));

                if (i_res.data == nullptr)
                    break;

                if (auto ptr =
                        dynamic_cast<IdentifierInitNode *>(i_res.data.get()))
                {
                    parameters.push_back(
                        AST::FunctionParameter{ptr->name(), ptr->mut_state(),
                                               std::move(ptr->type_ptr())});
                }

                expect_param = false;
            }

            if (parser.expect(Lexer::TokenTypes::Delimeter::Comma, false))
            {
                lexer.next();
                expect_param = true;

                continue;
            }

            break;
        }

        return parameters;
    }

    static Core::Position *parse_closing(Parser &parser, ParseResult &result,
                                         Lexer::TokenType expected)
    {
        auto &lexer = parser.lexer();

        if (auto diagnostic = parser.expect_or_error(expected, false))
        {
            parser.panic();

            result.status = Core::ResultStatus::Fail;
            result.diagnostics.push_back(std::move(diagnostic));

            return nullptr;
        }

        return &lexer.next()->end_position;
    }

    ParseResult
    FunctionDefinition::parse(Parser &parser,
                              AST::DeclarationAccessibility accessibility)
    {
        ParseResult result = parse(parser);

        auto decl =
            dynamic_cast<AST::FunctionDeclarationStmt *>(result.data.get());
        if (decl == nullptr)
            return result;

        decl->set_accessibility(accessibility);

        return result;
    }

    ParseResult FunctionDefinition::parse(Parser &parser)
    {
        // fn NAME -> RET_TYPE ([PARAM_LIST]) {}

        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        // fn (Keyword)
        if (auto diagnostic = parser.expect_or_error(token_type_, false))
            result.force_error(std::move(diagnostic));
        else
            lexer.next();

        // NAME (Identifier)
        Lexer::Token *name = nullptr;
        if (parser.expect(Primary::Identifier, false))
            name = lexer.next();
        else
        {
            auto node = AST::LiteralExpr(*lexer.peek());
            result.force_error(
                &node, Diagnostic::ErrorTypes::Syntax::MissingIdentifier,
                std::string("Missing function ") + Diagnostic::ERR_GEN +
                    "IDENTIFIER" + Utils::Styles::Reset + ".",
                "Missing identifier here");
        }

        // -> (ArrowLeftOperator)
        if (parser.expect(Operator::Arrow::Right, false))
            lexer.next();
        else
        {
            auto node = AST::LiteralExpr(*lexer.peek());
            result.force_error(&node,
                               Diagnostic::ErrorTypes::Syntax::MissingOperator,
                               std::string("Missing ") + Diagnostic::ERR_GEN +
                                   "->" + Utils::Styles::Reset + ".",
                               "Missing arrow operator here");
        }

        // RET_TYPE (ReturnType/Type)
        size_t rt_initial_pos = lexer.position();
        TypeParseResult rt_result = Common::Type.parse_type(parser);

        if (rt_result.status == Core::ResultStatus::Fail)
        {
            if (rt_initial_pos != lexer.position() &&
                !rt_result.diagnostics.empty())
                result.adapt(rt_result.status,
                             std::move(rt_result.diagnostics));

            else
            {
                auto node = AST::LiteralExpr(*lexer.peek());
                result.force_error(
                    &node, Diagnostic::ErrorTypes::Syntax::MissingReturnType,
                    std::string("Missing function ") + Diagnostic::ERR_GEN +
                        "RETURN TYPE" + Utils::Styles::Reset + ".",
                    "Missing return type here");
            }
        }

        // ( (ParenthesisOpen)
        if (parser.expect(Delimeter::ParenthesisOpen, false))
            lexer.next();
        else
            result.error(Diagnostic::create_syntax_error(
                lexer.peek(), Delimeter::ParenthesisOpen));

        // PARAM_LIST
        std::vector<AST::FunctionParameter> parameters =
            parse_parameters(parser, result);
        Core::Position *f_ep =
            parse_closing(parser, result, Delimeter::ParenthesisClose);

        // { (BraceOpen)
        if (parser.expect(Delimeter::BraceOpen, false))
        {
            ParseResult b_res = Common::Block.parse(parser);

            std::unique_ptr<AST::BlockStmt> body;
            if (b_res.data != nullptr)
                if (auto ptr =
                        dynamic_cast<AST::BlockStmt *>(b_res.data.release()))
                    body = std::unique_ptr<AST::BlockStmt>(ptr);

            result.adapt(b_res.status, std::move(b_res.diagnostics),
                         std::make_unique<AST::FunctionDefinitionStmt>(
                             *name, std::move(rt_result.data),
                             std::move(parameters), std::move(body)));

            return result;
        }
        else
        {
            // TERMINATOR ::= ";"
            ParseResult t_res = Common::Terminator.parse(parser);

            if (t_res.status == Core::ResultStatus::Fail)
            {
                result.status = t_res.status;
                result.data = nullptr;

                result.error(Diagnostic::create_syntax_error(
                    lexer.peek(), Delimeter::BraceOpen));
            }
            else
            {
                auto fn_node = std::make_unique<AST::FunctionDeclarationStmt>(
                    *name, std::move(rt_result.data), std::move(parameters));

                fn_node->set_end_position(*f_ep);
                result.data = std::move(fn_node);
            }

            result.adapt(std::move(t_res.diagnostics));

            return result;
        }
    }

    FunctionDefinitionReturn::FunctionDefinitionReturn()
        : GrammarRule(Lexer::TokenTypes::Reserved::Return)
    {
    }

    ParseResult FunctionDefinitionReturn::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        std::unique_ptr<AST::FunctionReturnStmt> node;

        // return (KeywordReturn)
        Core::Position *ret_pos;
        if (auto diagnostic = parser.expect_or_error(token_type_, false))
            result.diagnostics.push_back(std::move(diagnostic));
        else
        {
            Lexer::Token *ret = lexer.next();
            ret_pos = &ret->position;
        }

        // [VALUE] (ReturnValue)
        ExprParseResult v_res = Common::Expr.parse_expr(parser, 0);

        result.adapt(std::move(v_res.diagnostics));
        node = std::make_unique<AST::FunctionReturnStmt>(*ret_pos,
                                                         std::move(v_res.data));

        // ; (Terminator)
        ParseResult t_res = Common::Terminator.parse(parser);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        if (t_res.data != nullptr)
            node->set_end_position(t_res.data->end_position());

        result.data = std::move(node);

        return result;
    }

} // namespace Parser
