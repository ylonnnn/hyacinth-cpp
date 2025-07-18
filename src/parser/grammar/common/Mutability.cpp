#include <iostream>

#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Mutability.hpp"
#include "utils/style.hpp"

namespace Parser
{
    MutabilityNode::MutabilityNode(Core::Position position, bool mut)
        : Node(std::move(position)), mutable_(mut)
    {
    }

    void MutabilityNode::set_mutability(bool mut) { mutable_ = mut; }

    bool MutabilityNode::is_mutable() const { return mutable_; }

    void MutabilityNode::print(std::ostream &os, uint8_t tab) const
    {
        (void)os, (void)tab;
    }

    Mutability::Mutability() : GrammarRule(Lexer::TokenTypes::Delimeter::Colon)
    {
    }

    ParseResult Mutability::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Fail, nullptr, {}};

        Lexer::Token *token = lexer.peek();
        if (lexer.eof())
        {
            result.error(Diagnostic::create_syntax_error(token));
            return result;
        }

        auto mut = true;

        // Immutable
        if (parser.expect(token_type_, false))
        {
            mut = false;
            result.status = Core::ResultStatus::Success;

            lexer.next();
        }

        // Mutable
        else if (parser.expect(Lexer::TokenTypes::Operator::Logical::Not,
                               false))
        {
            lexer.next();

            if (!parser.expect(token_type_, false))
            {
                Lexer::Token *token = lexer.peek();

                auto diagnostic = Diagnostic::create_syntax_error(
                    token, Lexer::TokenTypes::Delimeter::Colon);
                auto node = new AST::LiteralExpr(*token);

                diagnostic->add_detail(
                    std::make_unique<Diagnostic::NoteDiagnostic>(
                        node, Diagnostic::NoteType::Suggestion,
                        std::string("Did you mean to put a \"") +
                            Diagnostic::NOTE_GEN + ":" + Utils::Styles::Reset +
                            "\"?",
                        "Replace this with (or Insert) a \":\" to make the "
                        "value mutable."));

                delete node;

                result.force_error(std::move(diagnostic));
            }

            else
            {
                lexer.next();

                result.status = Core::ResultStatus::Success;
            }
        }

        // Other (Throw an Error)
        else
        {
            result.data = nullptr;
            auto node = new AST::LiteralExpr(*token);

            result.force_error(
                node, Diagnostic::ErrorTypes::Syntax::MissingMutabilityModifier,
                std::string("Missing mutability modifier. Use \"") +
                    Diagnostic::ERR_GEN + ":" + Utils::Styles::Reset +
                    "\" for immutability, and \"" + Diagnostic::ERR_GEN +
                    "!:" + Utils::Styles::Reset + "\" for mutability.",
                "Use here");

            delete node;
        }

        result.data = std::make_unique<MutabilityNode>(token->position, mut);

        return result;
    }

} // namespace Parser
