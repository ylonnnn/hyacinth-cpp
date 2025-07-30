#include <iostream>

#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Mutability.hpp"
#include "utils/style.hpp"

namespace Parser
{
    MutabilityNode::MutabilityNode(Core::Position &position, bool mut)
        : Node(position), mutable_(mut)
    {
    }

    void MutabilityNode::set_mutability(bool mut) { mutable_ = mut; }

    bool MutabilityNode::is_mutable() const { return mutable_; }

    void MutabilityNode::print([[maybe_unused]] std::ostream &os,
                               [[maybe_unused]] uint8_t tab) const
    {
    }

    Mutability::Mutability() : GrammarRule(Lexer::TokenTypes::Delimeter::Colon)
    {
    }

    ParseResult Mutability::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Fail, nullptr, {}};

        Lexer::Token *token = lexer.peek();
        if (lexer.eof(false))
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
            if (parser.expect(token_type_))
                result.status = Core::ResultStatus::Success;

            else
            {
                // Lexer::Token *token = lexer.peek();
                Lexer::Token &token = lexer.current();

                auto diagnostic = Diagnostic::create_syntax_error(
                    &token, Lexer::TokenTypes::Delimeter::Colon);
                auto node = AST::LiteralExpr(token);

                diagnostic->add_detail(
                    std::make_unique<Diagnostic::NoteDiagnostic>(
                        &node, Diagnostic::NoteType::Suggestion,
                        std::string("Did you mean to put a \"") +
                            Diagnostic::NOTE_GEN + ":" + Utils::Styles::Reset +
                            "\"?",
                        "Replace this with (or Insert) a \":\" to make the "
                        "value mutable."));

                result.force_error(std::move(diagnostic));
            }
        }
        
        // test : i = 123;
        // test  i = 123;
        // test < i = 123;
        // test <> i = 123;

        // Other (Throw an Error)
        else
        {
            result.data = nullptr;
            auto node = AST::LiteralExpr(*token);

            result.force_error(
                &node,
                Diagnostic::ErrorTypes::Syntax::MissingMutabilityModifier,
                std::string("Missing mutability modifier. Use \"") +
                    Diagnostic::ERR_GEN + ":" + Utils::Styles::Reset +
                    "\" for immutability, and \"" + Diagnostic::ERR_GEN +
                    "!:" + Utils::Styles::Reset + "\" for mutability.",
                "Use here");
        }

        result.data = std::make_unique<MutabilityNode>(token->position, mut);

        return result;
    }

} // namespace Parser
