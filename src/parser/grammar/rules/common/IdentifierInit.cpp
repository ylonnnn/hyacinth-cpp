#include "parser/grammar/rules/common/IdentifierInit.hpp"
#include "ast/Node.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/rules/common/Mutability.hpp"

namespace Parser
{
    IdentifierNode::IdentifierNode(Lexer::Token &name, bool mut)
        : AST::Node(Program::Position(name.position)), name_(name),
          mutable_(mut)
    {
    }

    Lexer::Token &IdentifierNode::name() { return name_; }

    bool IdentifierNode::is_mutable() const { return mutable_; }

    void IdentifierNode::print(std::ostream &os, uint8_t tab) const {}

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

        Lexer::Token &name = lexer.current();

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

        bool mut = false;
        if (auto ptr = dynamic_cast<MutabilityNode *>(mut_node.get()))
            mut = ptr->is_mutable();

        // Identifier (TODO: Update to proper type)
        // Lexer::Token *type = nullptr;

        if (!parser.expect(token_type_))
        {
            result.status = ParseResultStatus::Failed;
            // type = &lexer.current();

            result.diagnostics.push_back(
                std::make_unique<Diagnostic::ErrorDiagnostic>(
                    std::make_unique<AST::LiteralExpr>(lexer.current()),
                    Diagnostic::ErrorTypes::General::Syntax,
                    std::string("Missing identifier ") + Diagnostic::ERR_GEN +
                        "TYPE" + Utils::Styles::Reset +
                        " after the mutability modifier.",
                    "Missing type here"));

            lexer.rewind(lexer.position() - 1);
        }

        result.node = std::make_unique<IdentifierNode>(name, mut);

        return result;
    }

} // namespace Parser
