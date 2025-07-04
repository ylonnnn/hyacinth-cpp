#include "parser/grammar/common/IdentifierInit.hpp"
#include "ast/Node.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/common/Mutability.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    IdentifierNode::IdentifierNode(Lexer::Token &name, bool mut,
                                   std::unique_ptr<AST::Type> type)
        : AST::Node(Program::Position(name.position)), name_(name),
          mutable_(mut), type_(std::move(type))
    {
    }

    Lexer::Token &IdentifierNode::name() { return name_; }

    std::unique_ptr<AST::Type> &IdentifierNode::type() { return type_; }

    bool IdentifierNode::is_mutable() const { return mutable_; }

    void IdentifierNode::print([[maybe_unused]] std::ostream &os,
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
        ParseResult result = {ParseResultStatus::Success, nullptr, {}};

        // Identifier
        if (parser.expect(token_type_, false))
            lexer.next();
        else
            result.force_error(
                parser, std::make_unique<AST::LiteralExpr>(*lexer.peek()),
                Diagnostic::ErrorTypes::Syntax::MissingIdentifier,
                std::string("Missing ") + Diagnostic::ERR_GEN + "IDENTIFIER" +
                    Utils::Styles::Reset + " before the mutability modifier.",
                "Missing identifier here");

        // if (auto diagnostic = parser.expect_or_error(token_type_))
        //     result.diagnostics.push_back(std::move(diagnostic));

        Lexer::Token &name = lexer.current();

        // Mutability Modifier
        auto [mut_status, mut_node, mut_diagnostics] =
            mutability_.parse(parser);

        if (mut_status == ParseResultStatus::Failed)
        {
            result.node = nullptr;
            result.status = mut_status;

            if (!mut_diagnostics.empty())
                result.diagnostics.insert(
                    result.diagnostics.end(),
                    std::make_move_iterator(mut_diagnostics.begin()),
                    std::make_move_iterator(mut_diagnostics.end()));
        }

        bool mut = false;
        if (auto ptr = dynamic_cast<MutabilityNode *>(mut_node.get()))
            mut = ptr->is_mutable();

        size_t t_initial_pos = lexer.position();
        TypeParseResult t_result = Common::Type.parse_type(parser);

        if (t_result.status == ParseResultStatus::Failed)
        {
            if (t_initial_pos != lexer.position() &&
                !t_result.diagnostics.empty())
            {
                result.status = t_result.status;
                result.diagnostics.insert(
                    result.diagnostics.end(),
                    std::make_move_iterator(t_result.diagnostics.begin()),
                    std::make_move_iterator(t_result.diagnostics.end()));
            }

            else
                result.force_error(
                    parser, std::make_unique<AST::LiteralExpr>(*lexer.peek()),
                    Diagnostic::ErrorTypes::Syntax::MissingType,
                    std::string("Missing identifier ") + Diagnostic::ERR_GEN +
                        "TYPE" + Utils::Styles::Reset +
                        " after the mutability modifier.",
                    "Missing type here");
        }

        result.node = std::make_unique<IdentifierNode>(
            name, mut, std::move(t_result.node));

        return result;
    }

} // namespace Parser
