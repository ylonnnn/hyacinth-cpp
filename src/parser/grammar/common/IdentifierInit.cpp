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
        : AST::Node(name.position), name_(name), mutable_(mut),
          type_(std::move(type))
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
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        // Identifier
        if (parser.expect(token_type_, false))
            lexer.next();
        else
        {
            auto node = new AST::LiteralExpr(*lexer.peek());

            result.force_error(
                node, Diagnostic::ErrorTypes::Syntax::MissingIdentifier,
                std::string("Missing ") + Diagnostic::ERR_GEN + "IDENTIFIER" +
                    Utils::Styles::Reset + " before the mutability modifier.",
                "Missing identifier here");

            delete node;
        }

        // if (auto diagnostic = parser.expect_or_error(token_type_))
        //     result.diagnostics.push_back(std::move(diagnostic));

        Lexer::Token &name = lexer.current();

        // Mutability Modifier
        auto mut_result = mutability_.parse(parser);

        if (mut_result.status == Core::ResultStatus::Fail)
        {
            result.data = nullptr;
            result.status = mut_result.status;

            if (!mut_result.diagnostics.empty())
                result.diagnostics.insert(
                    result.diagnostics.end(),
                    std::make_move_iterator(mut_result.diagnostics.begin()),
                    std::make_move_iterator(mut_result.diagnostics.end()));
        }

        bool mut = false;
        if (auto ptr = dynamic_cast<MutabilityNode *>(mut_result.data.get()))
            mut = ptr->is_mutable();

        size_t t_initial_pos = lexer.position();
        TypeParseResult t_result = Common::Type.parse_type(parser);

        if (t_result.status == Core::ResultStatus::Fail)
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
            {
                auto node = new AST::LiteralExpr(*lexer.peek());

                result.force_error(
                    node, Diagnostic::ErrorTypes::Syntax::MissingType,
                    std::string("Missing identifier ") + Diagnostic::ERR_GEN +
                        "TYPE" + Utils::Styles::Reset +
                        " after the mutability modifier.",
                    "Missing type here");

                delete node;
            }
        }

        result.data = std::make_unique<IdentifierNode>(
            name, mut, std::move(t_result.data));

        return result;
    }

} // namespace Parser
