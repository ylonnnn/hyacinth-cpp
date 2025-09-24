#include "parser/grammar/common/IdentifierInit.hpp"
#include "ast/Node.hpp"
#include "ast/common/Identifier.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/Mutability.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    IdentifierInitNode::IdentifierInitNode(
        Lexer::Token &name, AST::IdentifierMutabilityState mut_state,
        std::unique_ptr<AST::Type> type)
        : Node(name.position), AST::Identifier(name, mut_state, std::move(type))
    {
    }

    void IdentifierInitNode::print([[maybe_unused]] std::ostream &os,
                                   [[maybe_unused]] uint8_t tab) const
    {
    }

    IdentifierInitialization::IdentifierInitialization()
        : GrammarRule(Hyacinth::IDENTIFIER)
    {
    }

    ParseResult IdentifierInitialization::parse(Parser &parser)
    {
        // IDENTIFIER_INIT ::= IDENTNFIER (":" | "!:") TYPE

        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        // Identifier
        if (parser.expect(token_type_, false))
            lexer.next();
        else
        {
            auto node = AST::LiteralExpr(*lexer.peek());

            result.force_error(
                &node, Diagnostic::ErrorTypes::Syntax::MissingIdentifier,
                std::string("Missing ") + Diagnostic::ERR_GEN + "IDENTIFIER" +
                    utils::Styles::Reset + " before the mutability modifier.",
                "Missing identifier here");
        }

        // if (auto diagnostic = parser.expect_or_error(token_type_))
        //     result.diagnostics.push_back(std::move(diagnostic));

        Lexer::Token &name = lexer.current();

        // Mutability Modifier
        auto mut_result = mutability_.parse(parser);
        result.adapt(mut_result.status, std::move(mut_result.diagnostics));

        bool mut = false;
        auto m_node = mut_result.data.get();

        if (typeid(*m_node) == typeid(MutabilityNode))
            mut = static_cast<MutabilityNode *>(m_node)->is_mutable();

        size_t t_initial_pos = lexer.position();
        TypeParseResult t_result = Common::Type.parse_type(parser, 0);

        if (t_result.status == Core::ResultStatus::Fail)
        {
            if (t_initial_pos != lexer.position())
                result.adapt(t_result.status, std::move(t_result.diagnostics));

            else
            {
                auto node = AST::LiteralExpr(*lexer.peek());

                result.force_error(
                    &node, Diagnostic::ErrorTypes::Syntax::MissingType,
                    std::string("Missing identifier ") + Diagnostic::ERR_GEN +
                        "TYPE" + utils::Styles::Reset +
                        " after the mutability modifier.",
                    "Missing type here");
            }
        }

        if (result.status == Core::ResultStatus::Success)
            result.data = std::make_unique<IdentifierInitNode>(
                name,
                mut ? AST::IdentifierMutabilityState::Mutable
                    : AST::IdentifierMutabilityState::Immutable,
                std::move(t_result.data));

        return result;
    }

} // namespace Parser
