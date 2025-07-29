#include "parser/grammar/rules/Variable.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/ExprStmt.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "ast/stmt/variable/VariableDefStmt.hpp"
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
        // "let" IDENTIFIER (":" | "!:") TYPE
        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        result.diagnostics.reserve(16);

        // let (Reserved::Declaration)
        if (parser.expect(token_type_, false))
            lexer.next();
        else
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        // IDENTIFIER (":" | "!:") TYPE
        ParseResult i_res = Common::IdentifierInitialization.parse(parser);

        result.adapt(i_res.status, std::move(i_res.diagnostics));

        if (i_res.data == nullptr)
            return result;

        Lexer::Token *name = nullptr;
        AST::IdentifierMutabilityState mut_state =
            AST::IdentifierMutabilityState::Mutable;
        std::unique_ptr<AST::Type> type;

        if (i_res.data != nullptr)
            if (auto ptr = dynamic_cast<IdentifierInitNode *>(i_res.data.get()))
            {
                name = &ptr->name();
                mut_state = ptr->mut_state();
                type = std::move(ptr->type_ptr());
            }

        // (Optional) = VALUE
        auto presence_flag = 0;
        Lexer::Token *token = lexer.peek();

        if (parser.expect(Lexer::TokenTypes::Operator::Assignment::Direct,
                          false))
        {
            lexer.next();
            presence_flag |= (1 << 1);
        }

        ExprParseResult v_res = Common::Expr.parse_expr(parser, 0);

        result.adapt(std::move(v_res.diagnostics));

        if (v_res.data != nullptr)
            presence_flag |= (1 << 0);

        std::unique_ptr<AST::Expr> value = std::move(v_res.data);

        switch (presence_flag)
        {
            case 0b00:
            {
                result.data = std::make_unique<AST::VariableDeclarationStmt>(
                    *name, mut_state, std::move(type));

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
                    auto node = AST::LiteralExpr(*token);

                    result.force_error(
                        &node, Diagnostic::ErrorTypes::Syntax::MissingValue,
                        std::string("Missing ") + Diagnostic::ERR_GEN +
                            "VALUE" + Utils::Styles::Reset +
                            " after the assignment operator",
                        "Missing value after this operator");
                }

                break;
            }

            case 0b11:
            {
                if (v_res.status == Core::ResultStatus::Fail)
                    result.status = v_res.status;

                result.data = std::make_unique<AST::VariableDefinitionStmt>(
                    *name, mut_state, std::move(type), std::move(value));

                break;
            }
        }

        // TERMINATOR ::= ";"
        ParseResult t_res = Common::Terminator.parse(parser);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        if (auto ptr =
                dynamic_cast<AST::VariableDeclarationStmt *>(result.data.get()))
        {
            if (ptr->is_mutable() || ptr->is_definition())
                return result;

            auto node = AST::IdentifierExpr(*name);
            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &node,
                Diagnostic::ErrorTypes::Uninitialization::
                    UninitializedImmutable,
                std::string("Illegal uninitialization of immutable \"") +
                    Diagnostic::ERR_GEN + std::string(name->value) +
                    Utils::Styles::Reset + "\".",
                "Immutable variables require initial values");

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                result.data.get(), Diagnostic::NoteType::Declaration,
                std::string("Immutable variable \"") + Diagnostic::NOTE_GEN +
                    std::string(name->value) + Utils::Styles::Reset +
                    "\" declared here.",
                "Declared here"));

            result.force_error(std::move(diagnostic));
        }

        return result;
    }

} // namespace Parser
