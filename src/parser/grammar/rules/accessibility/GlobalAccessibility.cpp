#include "parser/grammar/rules/accessibility/GlobalAccessibility.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    using namespace Lexer::TokenTypes;
    GlobalAccessibility::GlobalAccessibility(Lexer::TokenType token_type)
        : GrammarRule(token_type)
    {
    }

    ParseResult GlobalAccessibility::parse(Parser &parser)
    {
        // pub|priv[GLOBAL_STMT]

        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        // pub|priv(Keyword)
        Lexer::Token *ac_token = nullptr;
        auto accessibility = AST::DeclarationAccessibility::Private;

        if (auto diagnostic = parser.expect_or_error(token_type_, false))
            result.force_error(std::move(diagnostic));
        else
        {
            ac_token = lexer.next();
            auto ac_type = *std::get_if<Reserved>(&ac_token->type);

            switch (ac_type)
            {
                case Reserved::Public:
                {
                    accessibility = AST::DeclarationAccessibility::Public;
                    break;
                }

                case Reserved::Private:
                {
                    accessibility = AST::DeclarationAccessibility::Private;
                    break;
                }

                default:
                    break;
            }
        }

        ParseResult p_res = parser.grammar().partial_parse(parser, true);
        std::unique_ptr<AST::Node> &p_data = p_res.data;

        auto decl = dynamic_cast<AST::DeclarationStmt *>(p_data.get());
        if (decl == nullptr)
        {
            result.error(Diagnostic::create_syntax_error(ac_token));

            return result;
        }

        decl->set_accessibility(accessibility);
        result.adapt(p_res.status, std::move(p_res.diagnostics),
                     std::move(p_data));

        return result;
    }

} // namespace Parser
