#include "parser/grammar/rules/Variable.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/rules/common/Common.hpp"
#include "utils/style.hpp"
#include <iostream>

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

        std::cout << "result.status: " << static_cast<int>(result.status)
                  << "\n";

        // (Optional) = VALUE
        // some_var: i32; some_var: i32 = 123;
        {
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

            // Conversion of Value Node
            std::unique_ptr<AST::Expr> expr;
            if (dynamic_cast<AST::Expr *>(v_node.get()))
                expr = std::unique_ptr<AST::Expr>(
                    static_cast<AST::Expr *>(v_node.release()));

            switch (presence_flag)
            {
                case 0b00:
                    break;

                case 0b01: // = [X] Value [/]
                case 0b10: // = [/] Value [X]
                {
                    result.status = ParseResultStatus::Failed;
                    result.node = nullptr;

                    result.diagnostics.push_back(
                        presence_flag == 0b01
                            ? std::make_unique<Diagnostic::ErrorDiagnostic>(
                                  std::move(expr),
                                  Diagnostic::ErrorTypes::General::Syntax,
                                  std::string("Missing ") +
                                      Diagnostic::ERR_GEN + "=" +
                                      Utils::Styles::Reset +
                                      " operator before the value",
                                  "Missing operator before this value")
                            : std::make_unique<Diagnostic::ErrorDiagnostic>(
                                  std::make_unique<AST::LiteralExpr>(*token),
                                  Diagnostic::ErrorTypes::General::Syntax,
                                  std::string("Missing ") +
                                      Diagnostic::ERR_GEN + "VALUE" +
                                      Utils::Styles::Reset +
                                      " after the assignment operator",
                                  "Missing value after this operator")

                    );

                    break;
                }

                case 0b11:
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

        std::cout << "variable (status): " << static_cast<int>(result.status)
                  << "\n";

        return result;
    }

} // namespace Parser
