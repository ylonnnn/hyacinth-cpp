#include <iostream>

#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Mutability.hpp"
#include "utils/style.hpp"

namespace Parser
{
    MutabilityNode::MutabilityNode(Program::Position position, bool mut)
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
        ParseResult result = {ParseResultStatus::Failed, nullptr, {}};

        Lexer::Token *token = lexer.peek();
        if (lexer.eof())
        {
            result.error(parser, Diagnostic::create_syntax_error(token));

            return result;
        }

        auto mut = true;

        // Immutable
        if (parser.expect(token_type_, false))
        {
            mut = false;
            result.status = ParseResultStatus::Success;

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

                result.force_error(
                    parser, Diagnostic::create_syntax_error(
                                token, Lexer::TokenTypes::Delimeter::Colon));

                result.note(std::make_unique<AST::LiteralExpr>(*token),
                            Diagnostic::NoteType::Suggestion,
                            std::string("Did you mean to put a \"") +
                                Diagnostic::NOTE_GEN + ":" +
                                Utils::Styles::Reset + "\"?",
                            "Replace this with (or Insert) a \":\" to make the "
                            "value mutable.");
            }

            else
            {
                lexer.next();

                result.status = ParseResultStatus::Success;
            }
        }

        // Other (Throw an Error)
        else
        {
            result.node = nullptr;

            result.force_error(
                parser, std::make_unique<AST::LiteralExpr>(*token),
                Diagnostic::ErrorTypes::Syntax::MissingMutabilityModifier,
                std::string("Missing mutability modifier. Use \"") +
                    Diagnostic::ERR_GEN + ":" + Utils::Styles::Reset +
                    "\" for immutability, and \"" + Diagnostic::ERR_GEN +
                    "!:" + Utils::Styles::Reset + "\" for mutability.",
                "Use here");
        }

        result.node = std::make_unique<MutabilityNode>(
            Program::Position(token->position), mut);

        return result;
    }

} // namespace Parser
