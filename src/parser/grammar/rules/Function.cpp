#include <iostream>

#include "ast/NodeCollection.hpp"
#include "ast/expr/LiteralExpr.hpp"
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

    FunctionParameterNode::FunctionParameterNode(
        Lexer::Token &name, bool mut, std::unique_ptr<AST::Type> type,
        std::unique_ptr<AST::Expr> default_value)
        : IdentifierNode(name, mut, std::move(type)),
          default_value_(std::move(default_value))
    {
    }

    std::unique_ptr<AST::Expr> &FunctionParameterNode::default_value()
    {
        return default_value_;
    }

    void FunctionParameterNode::print([[maybe_unused]] std::ostream &os,
                                      [[maybe_unused]] uint8_t tab) const
    {
    }

    FunctionParameterListParseResult::FunctionParameterListParseResult(
        Core::ResultStatus status,
        std::unique_ptr<AST::NodeCollection<FunctionParameterNode>> data,
        Diagnostic::DiagnosticList diagnostics)
        : Core::Result<
              std::unique_ptr<AST::NodeCollection<FunctionParameterNode>>>(
              status, std::move(data), std::move(diagnostics)) {};

    FunctionDefinition::FunctionDefinition() : GrammarRule(Hyacinth::FUNCTION)
    {
    }

    FunctionParameterListParseResult
    FunctionDefinition::parse_param_list(Parser &parser)
    {
        auto &lexer = parser.lexer();
        FunctionParameterListParseResult result = {
            Core::ResultStatus::Success, nullptr, {}};

        std::vector<std::unique_ptr<FunctionParameterNode>> parameters;
        parameters.reserve(8);

        auto &identifier = Common::IdentifierInitialization;
        auto expect_param = true;

        while (!parser.expect(Lexer::TokenTypes::Delimeter::ParenthesisClose,
                              false))
        {
            if (expect_param)
            {
                ParseResult i_res = identifier.parse(parser);

                if (i_res.data == nullptr)
                {
                    result.data = nullptr;
                    result.error(
                        Diagnostic::create_syntax_error(&lexer.current()));

                    break;
                }

                if (i_res.status == Core::ResultStatus::Fail)
                {
                    result.status = i_res.status;
                    result.diagnostics.insert(
                        result.diagnostics.end(),
                        std::make_move_iterator(i_res.diagnostics.begin()),
                        std::make_move_iterator(i_res.diagnostics.end()));
                }

                if (auto ptr = dynamic_cast<IdentifierNode *>(i_res.data.get()))
                {
                    parameters.push_back(
                        std::make_unique<FunctionParameterNode>(
                            ptr->name(), ptr->is_mutable(),
                            std::move(ptr->type()), nullptr));
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

        if (parameters.empty())
            parser.synchronize();
        else
            result.data =
                std::make_unique<AST::NodeCollection<FunctionParameterNode>>(
                    parameters[0]->position(), std::move(parameters));

        return result;
    }

    ParseResult FunctionDefinition::parse(Parser &parser)
    {
        // fn NAME -> RET_TYPE ([PARAM_LIST]) {}

        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        // fn (Keyword)
        if (auto diagnostic = parser.expect_or_error(token_type_, false))
            result.diagnostics.push_back(std::move(diagnostic));
        else
            lexer.next();

        // NAME (Identifier)
        Lexer::Token *name = nullptr;
        if (parser.expect(Primary::Identifier, false))
            name = lexer.next();
        else
        {
            auto node = new AST::LiteralExpr(*lexer.peek());
            result.force_error(
                node, Diagnostic::ErrorTypes::Syntax::MissingIdentifier,
                std::string("Missing function ") + Diagnostic::ERR_GEN +
                    "IDENTIFIER" + Utils::Styles::Reset +
                    " after function keyword.",
                "Missing identifier here");

            delete node;
        }

        // -> (ArrowLeftOperator)
        if (parser.expect(Operator::Arrow::Left, false))
            lexer.next();
        else
        {
            auto node = new AST::LiteralExpr(*lexer.peek());
            result.force_error(
                node, Diagnostic::ErrorTypes::Syntax::MissingOperator,
                std::string("Missing ") + Diagnostic::ERR_GEN + "->" +
                    Utils::Styles::Reset + " after function identifier.",
                "Missing arrow operator here");
        }

        // RET_TYPE (ReturnType/Type)
        size_t rt_initial_pos = lexer.position();
        ParseResult rt_result = Common::Type.parse_type(parser);

        if (rt_result.status == Core::ResultStatus::Fail)
        {
            if (rt_initial_pos != lexer.position() &&
                !rt_result.diagnostics.empty())
            {
                result.status = rt_result.status;
                result.diagnostics.insert(
                    result.diagnostics.end(),
                    std::make_move_iterator(rt_result.diagnostics.begin()),
                    std::make_move_iterator(rt_result.diagnostics.end()));
            }

            else
            {
                auto node = new AST::LiteralExpr(*lexer.peek());
                result.force_error(
                    node, Diagnostic::ErrorTypes::Syntax::MissingReturnType,
                    std::string("Missing function ") + Diagnostic::ERR_GEN +
                        "RETURN TYPE" + Utils::Styles::Reset +
                        " after function arrow operator.",
                    "Missing return type here");

                delete node;
            }
        }

        // ( (ParenthesisOpen)
        if (parser.expect(Delimeter::ParenthesisOpen, false))
            lexer.next();
        else
            result.error(Diagnostic::create_syntax_error(
                lexer.peek(), Delimeter::ParenthesisOpen));

        // PARAM_LIST
        size_t pl_initial_pos = lexer.position();
        FunctionParameterListParseResult pl_res = parse_param_list(parser);

        if (pl_res.status == Core::ResultStatus::Fail)
        {
            if (pl_initial_pos != lexer.position() &&
                !pl_res.diagnostics.empty())
            {
                result.status = pl_res.status;
                result.diagnostics.insert(
                    result.diagnostics.end(),
                    std::make_move_iterator(pl_res.diagnostics.begin()),
                    std::make_move_iterator(pl_res.diagnostics.end()));
            }

            else
                parser.synchronize();
        }

        // ) (ParenthesisClose)
        if (parser.expect(Delimeter::ParenthesisClose, false))
            lexer.next();
        else
            result.error(Diagnostic::create_syntax_error(
                lexer.peek(), Delimeter::ParenthesisClose));

        // { (BraceOpen)
        if (parser.expect(Delimeter::BraceOpen, false))
            lexer.next();
        else
            result.error(Diagnostic::create_syntax_error(lexer.peek(),
                                                         Delimeter::BraceOpen));

        // FUNCTION_BODY

        // } (BraceClose)
        if (parser.expect(Delimeter::BraceClose, false))
            lexer.next();
        else
            result.error(Diagnostic::create_syntax_error(
                lexer.peek(), Delimeter::BraceClose));

        return result;
    }

} // namespace Parser
