#include "parser/grammar/rules/Variable.hpp"
#include "ast/common/IdentifierDecl.hpp"
#include "ast/expr/Expr.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "ast/stmt/variable/VariableDefStmt.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/dev.hpp"
#include "utils/pointer.hpp"

namespace Parser
{
    VariableDefinition::VariableDefinition()
        : GrammarRule(Hyacinth::VARIABLE, GC_GLOBAL | GC_LOCAL)
    {
    }

    ParseResult VariableDefinition::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void VariableDefinition::parse(Parser &parser, ParseResult &result)
    {
        using Lexer::TokenType;

        // "var" "constexpr"? "mut"? IDENTIFIER ( ":" TYPE )? ( "=" VALUE )? ;

        auto &lexer = parser.lexer;
        result.diagnostics.reserve(16);

        // var (TokenType::Var)
        lexer.consume();

        // constexpr ? (TokenType::Constexpr)
        bool is_constexpr = false;
        if (parser.expect(Lexer::TokenType::Constexpr, false))
        {
            lexer.consume();
            is_constexpr = true;
        }

        // mut? (TokenType::Mut)
        AST::IdentifierMutabilityState mut_state =
            AST::IdentifierMutabilityState::Immutable;
        if (parser.expect(TokenType::Mut, false))
        {
            lexer.consume();
            mut_state = AST::IdentifierMutabilityState::Mutable;
        }

        // IDENTIFIER
        Lexer::Token *identifier =
            parser.expect_or_error(TokenType::Identifier, result);
        if (identifier == nullptr)
            return;

        auto flag = 0;

        // ":" TYPE
        std::unique_ptr<AST::Type> type;
        if (parser.expect(TokenType::Colon, false))
        {
            flag |= (1 << 0);
            lexer.consume();

            ParseResult t_res = Common::Pratt.parse_base(parser, 0, true);
            result.adapt(t_res.status, std::move(t_res.diagnostics));

            type = utils::dynamic_ptr_cast<AST::Type>(t_res.data);
        }

        // "=" VALUE
        std::unique_ptr<AST::Expr> value;
        if (parser.expect(TokenType::Equal, false))
        {
            flag |= (1 << 1);
            lexer.consume();

            ParseResult v_res = Common::Pratt.parse_base(parser, 0);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            value = utils::dynamic_ptr_cast<AST::Expr>(v_res.data);
            if (value == nullptr)
            {
                Lexer::Token &prev = lexer.current(), *token = lexer.peek();
                if (token == nullptr)
                    return;

                result.force_error(
                    Core::range_between(prev.range.end(), token->range.start()),
                    Diagnostic::ErrorType::MissingValue,
                    "missing value expression.");

                return;
            }
        }

        // Invalid Form
        if (flag == 0)
        {
            utils::todo("throw error: either a value or an explicit type "
                        "annotation must be present.");
            return;
        }

        // ;
        Common::Terminator.parse(parser, result);

        if (result.status == Core::ResultStatus::Fail)
            return;

        // Distinction

        // Declaration
        if (value == nullptr)
        {
            if (is_constexpr)
            {
                result.error(identifier->range,
                             Diagnostic::ErrorType::NonConstantEvaluatable,
                             "variable declaration cannot be 'constexpr'.");

                return;
            }
            result.data = std::make_unique<AST::VariableDeclarationStmt>(
                *identifier, mut_state, std::move(type));
        }

        // Definition
        else
            result.data = std::make_unique<AST::VariableDefinitionStmt>(
                *identifier, is_constexpr, mut_state, std::move(type),
                std::move(value));
    }

    void VariableDefinition::recover(Parser &parser)
    {
        using Lexer::TokenType;

        parser.synchronize({TokenType::Semicolon});
    }

} // namespace Parser
