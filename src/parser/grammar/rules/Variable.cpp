#include "parser/grammar/rules/Variable.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
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

    ParseResult
    VariableDefinition::parse(Parser &parser,
                              AST::DeclarationAccessibility accessibility)
    {
        ParseResult result = parse(parser);

        auto decl =
            dynamic_cast<AST::VariableDeclarationStmt *>(result.data.get());
        if (decl == nullptr)
            return result;

        decl->set_accessibility(accessibility);
        return result;
    }

    ParseResult VariableDefinition::parse(Parser &parser)
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

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

        // IDENTIFIER
        Lexer::Token *identifier = nullptr;
        if (auto diagnostic =
                parser.expect_or_error(Primary::Identifier, false))
            result.force_error(std::move(diagnostic));
        else
            identifier = lexer.next();

        // ":" | "!:"
        ParseResult m_res = Common::Mutability.parse(parser);
        result.adapt(m_res.status, std::move(m_res.diagnostics));

        auto m_node = m_res.data.get();
        auto mut_state = AST::IdentifierMutabilityState::Immutable;
        Core::Position *ms_pos = nullptr;

        if (typeid(*m_node) == typeid(MutabilityNode))
        {
            auto node = static_cast<MutabilityNode *>(m_node);

            if (node->is_mutable())
                mut_state = AST::IdentifierMutabilityState::Mutable;

            ms_pos = &node->end_position();
        }

        // (Optional) TYPE
        TypeParseResult t_res = Common::Type.parse_type(parser, 0);
        std::unique_ptr<AST::Type> type = std::move(t_res.data);

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
                    *identifier, mut_state, std::move(type));

                result.data->set_end_position(*ms_pos);

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
                            utils::Styles::Reset + " operator.",
                        "Missing operator before this value");

                else
                {
                    auto node = AST::LiteralExpr(*token);

                    result.force_error(
                        &node, Diagnostic::ErrorTypes::Syntax::MissingValue,
                        std::string("Missing ") + Diagnostic::ERR_GEN +
                            "VALUE" + utils::Styles::Reset + ".",
                        "Missing value after this operator");
                }

                break;
            }

            case 0b11:
            {
                if (v_res.status == Core::ResultStatus::Fail)
                    result.status = v_res.status;

                result.data = std::make_unique<AST::VariableDefinitionStmt>(
                    *identifier, mut_state, std::move(type), std::move(value));

                break;
            }
        }

        // TERMINATOR ::= ";"
        ParseResult tp_res = Common::Terminator.parse(parser);
        result.adapt(tp_res.status, std::move(tp_res.diagnostics));

        if (auto ptr =
                dynamic_cast<AST::VariableDeclarationStmt *>(result.data.get()))
        {
            if (ptr->is_mutable() || ptr->is_definition())
                return result;

            auto node = AST::IdentifierExpr(*identifier);
            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &node,
                Diagnostic::ErrorTypes::Uninitialization::
                    UninitializedImmutable,
                std::string("Illegal uninitialization of immutable \"") +
                    Diagnostic::ERR_GEN + std::string(identifier->value) +
                    utils::Styles::Reset + "\".",
                "Immutable variables require initial values");

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                result.data.get(), Diagnostic::NoteType::Declaration,
                std::string("Immutable variable \"") + Diagnostic::NOTE_GEN +
                    std::string(identifier->value) + utils::Styles::Reset +
                    "\" declared here.",
                "Declared here"));

            result.force_error(std::move(diagnostic));
        }

        return result;
    }

} // namespace Parser
