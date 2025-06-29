#include "parser/grammar/rules/Variable.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/VariableDeclStmt.hpp"
#include "ast/stmt/VariableDefStmt.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/rules/common/Common.hpp"
#include "parser/grammar/rules/common/IdentifierInit.hpp"
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
        if (auto ptr = dynamic_cast<IdentifierNode *>(result.node.get()))
            identifier = std::unique_ptr<IdentifierNode>(
                static_cast<IdentifierNode *>(result.node.release()));

        // (Optional) = VALUE
        auto presence_flag = 0;
        Lexer::Token *token = lexer.peek();

        if (parser.expect(Lexer::TokenTypes::Operator::Assignment::Direct,
                          false))
        {
            lexer.next();
            presence_flag |= (1 << 1);
        }

        auto [v_status, v_node, v_diagnostics] =
            parser.grammar().fallback()->parse(parser);

        if (v_status != ParseResultStatus::Failed)
            presence_flag |= (1 << 0);

        Lexer::Token &name = identifier->name();
        AST::VariableMutabilityState mut_state =
            identifier->is_mutable() ? AST::VariableMutabilityState::Mutable
                                     : AST::VariableMutabilityState::Immutable;

        std::unique_ptr<AST::Expr> value;
        if (dynamic_cast<AST::Expr *>(v_node.get()))
            value = std::unique_ptr<AST::Expr>(
                static_cast<AST::Expr *>(v_node.release()));

        switch (presence_flag)
        {
            case 0b00:
            {
                result.node = std::make_unique<AST::VariableDeclarationStmt>(
                    name, mut_state);

                break;
            }

            case 0b01: // = [X] Value [/]
            case 0b10: // = [/] Value [X]
            {
                result.status = ParseResultStatus::Failed;
                result.node = nullptr;

                result.diagnostics.push_back(
                    presence_flag == 0b01
                        ? std::make_unique<Diagnostic::ErrorDiagnostic>(
                              std::move(value),
                              Diagnostic::ErrorTypes::General::Syntax,
                              std::string("Missing ") + Diagnostic::ERR_GEN +
                                  "=" + Utils::Styles::Reset +
                                  " operator before the value",
                              "Missing operator before this value")
                        : std::make_unique<Diagnostic::ErrorDiagnostic>(
                              std::make_unique<AST::LiteralExpr>(*token),
                              Diagnostic::ErrorTypes::General::Syntax,
                              std::string("Missing ") + Diagnostic::ERR_GEN +
                                  "VALUE" + Utils::Styles::Reset +
                                  " after the assignment operator",
                              "Missing value after this operator")

                );

                break;
            }

            case 0b11:
            {
                result.node = std::make_unique<AST::VariableDefinitionStmt>(
                    name, mut_state, std::move(value));
                break;
            }
        }

        // TERMINATOR ::= ";"
        auto [t_status, _, t_diagnostics] = Common::Terminator.parse(parser);

        if (t_status == ParseResultStatus::Failed)
        {
            result.status = t_status;
            result.node = nullptr;

            if (!t_diagnostics.empty())
                result.diagnostics.insert(
                    result.diagnostics.end(),
                    std::make_move_iterator(t_diagnostics.begin()),
                    std::make_move_iterator(t_diagnostics.end()));
        }

        if (auto ptr =
                dynamic_cast<AST::VariableDeclarationStmt *>(result.node.get()))
        {
            std::cout << "is mutable: " << ptr->is_mutable() << "\n";
            if (ptr->is_mutable() || ptr->is_definition())
                return result;

            result.status = ParseResultStatus::Failed; 
            // result.node = nullptr;

            result.diagnostics.push_back(
                std::make_unique<Diagnostic::ErrorDiagnostic>(
                    std::make_unique<AST::IdentifierExpr>(name),
                    Diagnostic::ErrorTypes::Uninitialization::
                        UninitializedImmutable,
                    std::string("Illegal uninitialization of immutable \"") +
                        Diagnostic::ERR_GEN + std::string(name.value) +
                        Utils::Styles::Reset + "\".",
                    "Immutable variables require initial values"));

            result.diagnostics.push_back(
                std::make_unique<Diagnostic::NoteDiagnostic>(
                    std::move(result.node), Diagnostic::NoteType::Definition,
                    std::string("Immutable variable \"") +
                        Diagnostic::NOTE_GEN + std::string(name.value) +
                        Utils::Styles::Reset + "\" defined here",
                    "Defined here"));
        }

        return result;
    }

} // namespace Parser
