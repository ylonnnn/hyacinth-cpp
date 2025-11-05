#include "parser/grammar/rules/Import.hpp"
#include "ast/stmt/ImportStmt.hpp"
#include "parser/grammar/GrammarContext.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    using Lexer::TokenType;

    ImportStatement::ImportStatement()
        : GrammarRule(Hyacinth::IMPORT, GC_GLOBAL)
    {
    }

    std::vector<Lexer::Token *>
    ImportStatement::parse_imported_symbols(Parser &parser, ParseResult &result)
    {
        auto &lexer = parser.lexer;

        std::vector<Lexer::Token *> symbols;
        symbols.reserve(8);

        auto closing = TokenType::RightBrace;
        auto expect_sym = true;

        while (!parser.expect(closing, false))
        {
            if (expect_sym)
            {
                if (auto symbol =
                        parser.expect_or_error(TokenType::Identifier, result))
                    symbols.push_back(symbol);

                expect_sym = false;
            }

            if (parser.expect(TokenType::Comma, false))
            {
                lexer.consume();
                expect_sym = true;

                continue;
            }

            break;
        }

        return symbols;
    }

    ParseResult ImportStatement::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void ImportStatement::parse(Parser &parser, ParseResult &result)
    {
        // import "TARGET" "->" "{" SYMBOLS "}" ";"

        auto &lexer = parser.lexer;

        // import
        lexer.consume();

        // "TARGET" (TokenType::String)
        Lexer::Token *target =
            parser.expect_or_error(TokenType::String, result);
        if (target == nullptr)
            return;

        // ->
        parser.expect_or_error(TokenType::MinusGreater, result);

        // {
        parser.expect_or_error(TokenType::LeftBrace, result);

        // SYMBOLS*
        std::vector<Lexer::Token *> symbols =
            parse_imported_symbols(parser, result);

        // }
        Core::Position *e_pos = nullptr;
        if (auto cl = parser.expect_or_error(TokenType::LeftBrace, result))
            e_pos = &cl->range.end();

        // ;
        ParseResult t_res = Common::Terminator.parse(parser);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        if (result.status != Core::ResultStatus::Fail)
        {
            result.data =
                std::make_unique<AST::ImportStmt>(*target, std::move(symbols));
            result.data->range.end(*e_pos);
        }
    }

    void ImportStatement::recover(Parser &parser)
    {
        parser.synchronize({TokenType::RightBrace, TokenType::Semicolon});
    }

} // namespace Parser
