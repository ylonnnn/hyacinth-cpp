#include "parser/grammar/rules/Variable.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/VariableDeclStmt.hpp"
#include "ast/stmt/VariableDefStmt.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/IdentifierInit.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/style.hpp"

namespace Parser
{
    VariableDefinition::VariableDefinition() : GrammarRule(Hyacinth::VARIABLE)
    {
    }

    ParseResult VariableDefinition::parse(Parser &parser)
    {
        // IDENTIFIER (":" | "!:") TYPE
        auto &lexer = parser.lexer();
        ParseResult result = Common::IdentifierInitialization.parse(parser);

        std::unique_ptr<IdentifierNode> identifier;
        if (dynamic_cast<IdentifierNode *>(result.data.get()))
            identifier = std::unique_ptr<IdentifierNode>(
                static_cast<IdentifierNode *>(result.data.release()));

        // (Optional) = VALUE
        auto presence_flag = 0;
        Lexer::Token *token = lexer.peek();

        if (parser.expect(Lexer::TokenTypes::Operator::Assignment::Direct,
                          false))
        {
            lexer.next();
            presence_flag |= (1 << 1);
        }

        ParseResult v_res = parser.grammar().fallback()->parse(parser);

        if (v_res.status != Core::ResultStatus::Fail)
            presence_flag |= (1 << 0);

        Lexer::Token &name = identifier->name();
        AST::VariableMutabilityState mut_state =
            identifier->is_mutable() ? AST::VariableMutabilityState::Mutable
                                     : AST::VariableMutabilityState::Immutable;
        std::unique_ptr<AST::Type> &type = identifier->type();

        std::unique_ptr<AST::Expr> value;
        if (dynamic_cast<AST::Expr *>(v_res.data.get()))
            value = std::unique_ptr<AST::Expr>(
                static_cast<AST::Expr *>(v_res.data.release()));

        switch (presence_flag)
        {
            case 0b00:
            {
                result.data = std::make_unique<AST::VariableDeclarationStmt>(
                    name, mut_state, std::move(type));

                break;
            }

            case 0b01: // = [X] Value [/]
            case 0b10: // = [/] Value [X]
            {
                result.data = nullptr;

                if (presence_flag == 0b01)
                    result.force_error(
                        value.get(),
                        Diagnostic::ErrorTypes::Syntax::MissingOperator,
                        std::string("Missing ") + Diagnostic::ERR_GEN + "=" +
                            Utils::Styles::Reset + " operator before the value",
                        "Missing operator before this value");

                else
                {
                    auto node = new AST::LiteralExpr(*token);

                    result.force_error(
                        node, Diagnostic::ErrorTypes::Syntax::MissingValue,
                        std::string("Missing ") + Diagnostic::ERR_GEN +
                            "VALUE" + Utils::Styles::Reset +
                            " after the assignment operator",
                        "Missing value after this operator");

                    delete node;
                }

                break;
            }

            case 0b11:
            {
                result.data = std::make_unique<AST::VariableDefinitionStmt>(
                    name, mut_state, std::move(type), std::move(value));

                break;
            }
        }

        // TERMINATOR ::= ";"
        ParseResult t_res = Common::Terminator.parse(parser);

        if (t_res.status == Core::ResultStatus::Fail)
        {
            result.status = t_res.status;
            result.data = nullptr;

            if (!t_res.diagnostics.empty())
                result.diagnostics.insert(
                    result.diagnostics.end(),
                    std::make_move_iterator(t_res.diagnostics.begin()),
                    std::make_move_iterator(t_res.diagnostics.end()));
        }

        if (auto ptr =
                dynamic_cast<AST::VariableDeclarationStmt *>(result.data.get()))
        {
            if (ptr->is_mutable() || ptr->is_definition())
                return result;

            auto node = new AST::IdentifierExpr(name);
            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                node,
                Diagnostic::ErrorTypes::Uninitialization::
                    UninitializedImmutable,
                std::string("Illegal uninitialization of immutable \"") +
                    Diagnostic::ERR_GEN + std::string(name.value) +
                    Utils::Styles::Reset + "\".",
                "Immutable variables require initial values");

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                result.data.get(), Diagnostic::NoteType::Declaration,
                std::string("Immutable variable \"") + Diagnostic::NOTE_GEN +
                    std::string(name.value) + Utils::Styles::Reset +
                    "\" declared here.",
                "Declared here"));

            delete node;

            result.force_error(std::move(diagnostic));
        }

        return result;
    }

} // namespace Parser
