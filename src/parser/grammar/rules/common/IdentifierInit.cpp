#include "parser/grammar/rules/common/IdentifierInit.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/rules/common/Mutability.hpp"

namespace Parser
{
    IdentifierInitialization::IdentifierInitialization()
        : GrammarRule(Hyacinth::IDENTIFIER)
    {
    }

    ParseResult IdentifierInitialization::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();
        ParseResult result = {ParseResultStatus::Success, nullptr, {}};

        // IDENTIFIER_INIT ::= IDENTNFIER (":" | "!:") TYPE

        // Identifier
        if (auto diagnostic = parser.expect_or_error(token_type_))
            result.diagnostics.push_back(std::move(diagnostic));

        // Mutability Modifier
        auto [mut_status, mut_node, mut_diagnostics] =
            mutability_.parse(parser);

        result.status = mut_status;
        result.diagnostics.insert(
            result.diagnostics.end(),
            std::make_move_iterator(mut_diagnostics.begin()),
            std::make_move_iterator(mut_diagnostics.end()));

        if (result.status == ParseResultStatus::Failed)
            result.node = nullptr;

        // Identifier (TODO: Update to proper type)
        if (!parser.expect(token_type_))
        {
            result.status = ParseResultStatus::Failed;

            result.diagnostics.push_back(
                std::make_unique<Diagnostic::ErrorDiagnostic>(
                    std::make_unique<AST::LiteralExpr>(
                        parser.lexer().current()),
                    Diagnostic::ErrorTypes::General::Syntax,
                    std::string("Missing identifier ") + Diagnostic::ERR_GEN +
                        "TYPE" + Utils::Styles::Reset +
                        " after the mutability modifier.",
                    "Missing type here"));

            lexer.rewind(lexer.position() - 1);
        }

        return result;
    }

} // namespace Parser
