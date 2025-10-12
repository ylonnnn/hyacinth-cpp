#include "parser/grammar/rules/accessibility/GlobalAccessibility.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "diagnostic/helpers.hpp"
#include "lexer/Token.hpp"
#include "parser/grammar/GrammarContext.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    GlobalAccessibility::GlobalAccessibility(Lexer::TokenType token_type)
        : GrammarRule(token_type, GC_GLOBAL)
    {
    }

    ParseResult GlobalAccessibility::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void GlobalAccessibility::parse(Parser &parser, ParseResult &result)
    {
        // pub [GLOBAL_STMT]

        auto &lexer = parser.lexer;

        // pub
        Lexer::Token *token = lexer.next();
        auto accessibility = AST::DeclarationAccessibility::Private;

        switch (token->type)
        {
            case Lexer::TokenType::Pub:
                accessibility = AST::DeclarationAccessibility::Public;
                break;

            default:
                break;
        }

        ParseResult p_res = parser.grammar.partial_parse(parser, GC_GLOBAL);
        result.adapt(p_res.status, std::move(p_res.diagnostics));

        if (p_res.data == nullptr)
            return;

        auto decl = dynamic_cast<AST::DeclarationStmt *>(p_res.data.get());
        if (decl == nullptr)
        {
            result.error(Diagnostic::create_syntax_error(*token));
            return;
        }

        decl->accessibility = accessibility;
        result.data = std::move(p_res.data);
    }

} // namespace Parser
