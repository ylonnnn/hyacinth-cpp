#include "parser/grammar/rules/Import.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/ImportStmt.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    using namespace Lexer::TokenTypes;

    ImportStatement::ImportStatement() : GrammarRule(Hyacinth::IMPORT) {}

    ParseResult ImportStatement::parse(Parser &parser)
    {
        // import "TARGET" ;

        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        // import (KeywordImport)
        if (auto diagnostic = parser.expect_or_error(token_type_, false))
            result.force_error(std::move(diagnostic));
        else
            lexer.next();

        // "TARGET" (String)
        Lexer::Token *target = nullptr;
        if (parser.expect(Primary::String, false))
            target = lexer.next();
        else
        {
            auto node = AST::LiteralExpr(*lexer.peek());
            result.force_error(
                &node, Diagnostic::ErrorTypes::Syntax::MissingImportTarget,
                std::string("Missing ") + Diagnostic::ERR_GEN +
                    "IMPORT TARGET" + Utils::Styles::Reset + ".",
                "Missing import target here");
        }

        // ; (Terminator)
        ParseResult t_res = Common::Terminator.parse(parser);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        result.data = std::make_unique<AST::ImportStmt>(*target);

        return result;
    }

} // namespace Parser
