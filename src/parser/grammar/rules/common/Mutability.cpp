#include "parser/grammar/rules/common/Mutability.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "utils/style.hpp"
#include <iostream>

namespace Parser
{
    MutabilityNode::MutabilityNode(Program::Position position, bool mut)
        : Node(std::move(position)), mutable_(mut)
    {
    }

    void MutabilityNode::set_mutability(bool mut) { mutable_ = mut; }

    bool MutabilityNode::is_mutable() const { return mutable_; }

    void MutabilityNode::print(std::ostream &os, uint8_t tab) const {}

    Mutability::Mutability() : GrammarRule(Lexer::TokenTypes::Delimeter::Colon)
    {
    }

    ParseResult Mutability::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();
        ParseResult result = {ParseResultStatus::Failed, nullptr, {}};

        size_t initial_pos = lexer.position();

        Lexer::Token *token = lexer.peek();
        if (lexer.eof())
        {
            result.diagnostics.push_back(
                Diagnostic::create_syntax_error(token));

            return result;
        }

        auto mut = false;

        // Immutable
        if (parser.expect(token_type_, false))
        {
            result.status = ParseResultStatus::Success;
            lexer.next();
        }

        // Mutable
        else if (parser.expect(Lexer::TokenTypes::Operator::Logical::Not,
                               false))
        {
            lexer.next();
            mut = true;

            if (!parser.expect(token_type_, false))
            {
                Lexer::Token *token = lexer.peek();

                result.diagnostics.push_back(Diagnostic::create_syntax_error(
                    token, Lexer::TokenTypes::Delimeter::Colon));

                result.diagnostics.push_back(
                    std::make_unique<Diagnostic::NoteDiagnostic>(
                        std::make_unique<AST::LiteralExpr>(*token),
                        Diagnostic::NoteType::Suggestion,
                        std::string("Did you mean to put a \"") +
                            Diagnostic::NOTE_GEN + ":" + Utils::Styles::Reset +
                            "\"?",
                        "Replace this with (or Insert) a \":\" to make the "
                        "value mutable."));
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

            result.diagnostics.push_back(
                std::make_unique<Diagnostic::ErrorDiagnostic>(
                    std::make_unique<AST::LiteralExpr>(*token),
                    Diagnostic::ErrorTypes::General::Syntax,
                    std::string("Missing mutability modifier. Use \"") +
                        Diagnostic::ERR_GEN + ":" + Utils::Styles::Reset +
                        "\" for immutability, and \"" + Diagnostic::ERR_GEN +
                        "!:" + Utils::Styles::Reset + "\" for mutability.",
                    "Use here"));
        }

        result.node = std::make_unique<MutabilityNode>(
            Program::Position(token->position), mut);

        return result;
    }

} // namespace Parser
