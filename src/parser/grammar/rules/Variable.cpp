// #include "parser/grammar/rules/Variable.hpp"
// #include "ast/expr/IdentifierExpr.hpp"
// #include "ast/expr/LiteralExpr.hpp"
// #include "ast/stmt/variable/VariableDeclStmt.hpp"
// #include "ast/stmt/variable/VariableDefStmt.hpp"
// #include "diagnostic/ErrorDiagnostic.hpp"
// #include "diagnostic/NoteDiagnostic.hpp"
// #include "parser/grammar/GrammarRule.hpp"
// #include "parser/grammar/common/Common.hpp"
// #include "parser/grammar/common/IdentifierInit.hpp"
// #include "parser/grammar/rules/Hyacinth.hpp"
// #include "utils/style.hpp"

#include "parser/grammar/rules/Variable.hpp"
#include "ast/common/IdentifierDecl.hpp"
#include "ast/expr/Expr.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "ast/stmt/variable/VariableDefStmt.hpp"
#include "diagnostic/helpers.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/dev.hpp"

namespace Parser
{
    VariableDefinition::VariableDefinition()
        : GrammarRule(Hyacinth::VARIABLE, GC_GLOBAL | GC_LOCAL)
    {
    }

    ParseResult
    VariableDefinition::parse(Parser &parser,
                              AST::DeclarationAccessibility accessibility)
    {
        utils::todo("implement variable access modifiers");
        return parse(parser);
    }

    ParseResult VariableDefinition::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void VariableDefinition::parse(Parser &parser, ParseResult &result)
    {
        using TokenType = Lexer::TokenType;

        // "var" "mut"? IDENTIFIER ( ":" TYPE )? ( "=" VALUE )? ;

        auto &lexer = parser.lexer;
        result.diagnostics.reserve(16);

        // var (TokenType::Var)
        lexer.consume();

        // mut? (TokenType::Mut)
        AST::IdentifierMutabilityState mut_state =
            AST::IdentifierMutabilityState::Immutable;
        if (parser.expect(TokenType::Mut, false))
        {
            lexer.consume();
            mut_state = AST::IdentifierMutabilityState::Mutable;
        }

        // IDENTIFIER (TokenType::Identifier)
        Lexer::Token *identifier = nullptr;
        if (auto diagnostic =
                parser.expect_or_error(TokenType::Identifier, false))
            result.force_error(std::move(diagnostic));
        else
            identifier = lexer.next();

        auto flag = 0;

        // ":" TYPE
        std::unique_ptr<AST::Path> type;
        if (parser.expect(TokenType::Colon, false))
        {
            flag |= (1 << 0);
            lexer.consume();

            if (!parser.expect(TokenType::Identifier, false))
            {
                Lexer::Token *token = lexer.peek();

                result.error(
                    Diagnostic::create_unexpected_token_error(*token, "type"));
            }
            else
            {
                PrattParseResult t_res = Common::Pratt.parse_base(parser, 0);
                result.adapt(t_res.status, std::move(t_res.diagnostics));

                auto ptr = dynamic_cast<AST::Path *>(t_res.data.get());
                if (ptr == nullptr)
                {
                    utils::todo("throw error: type must be an identifier or a "
                                "path to an identified type");
                    return;
                }

                type = std::unique_ptr<AST::Path>(ptr);

                auto _ = t_res.data.release();
                (void)_;
            }
        }

        // "=" VALUE
        std::unique_ptr<AST::Expr> value;
        if (parser.expect(TokenType::Equal, false))
        {
            flag |= (1 << 1);
            lexer.consume();

            PrattParseResult v_res = Common::Pratt.parse_base(parser, 0);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            auto ptr = dynamic_cast<AST::Expr *>(v_res.data.get());
            if (ptr == nullptr)
            {
                utils::todo("throw error: value must be an expression");
                return;
            }

            value = std::unique_ptr<AST::Expr>(ptr);

            auto _ = v_res.data.release();
            (void)_;
        }

        // Invalid Form
        if (flag == 0)
        {
            utils::todo("throw error: either a value or an explicit type "
                        "annotation must be present.");
            return;
        }

        if (auto diagnostic =
                parser.expect_or_error(TokenType::Semicolon, false))
            result.error(std::move(diagnostic));
        else
            lexer.consume();

        // Distinction between declaration and definition

        // Declaration
        if (value == nullptr)
            result.data = std::make_unique<AST::VariableDeclarationStmt>(
                *identifier, mut_state, std::move(type));

        // Definition
        else
            result.data = std::make_unique<AST::VariableDefinitionStmt>(
                *identifier, mut_state, std::move(type), std::move(value));
    }

} // namespace Parser
