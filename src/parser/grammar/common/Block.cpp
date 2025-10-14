#include "parser/grammar/common/Block.hpp"
#include "ast/stmt/BlockStmt.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    Block::Block() : GrammarRule(Hyacinth::BLOCK, GC_LOCAL) {}

    ParseResult Block::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    /**
     * Default Local Block Parsing
     */
    void Block::parse(Parser &parser, ParseResult &result)
    {
        parse(parser, GC_LOCAL, result);
    }

    void Block::parse(Parser &parser, GrammarContext context,
                      ParseResult &result)
    {
        // "{" STATEMENT* "}"

        auto &lexer = parser.lexer;

        // {
        Core::Position &pos = lexer.next()->range.start;

        auto block = std::make_unique<AST::BlockStmt>(
            pos, std::vector<std::unique_ptr<AST::Stmt>>{});
        block->statements.reserve(8);

        Lexer::TokenType closing = Lexer::TokenType::RightBrace;

        // STATEMENT*
        while (!parser.expect(closing, false))
        {
            ParseResult p_res = parser.grammar.partial_parse(parser, context);
            result.adapt(p_res.status, std::move(p_res.diagnostics));

            auto stmt = dynamic_cast<AST::Stmt *>(p_res.data.get());
            if (stmt == nullptr)
                break;

            // Release (Stored to avoid warnings)
            auto _ = p_res.data.release();
            (void)_;

            block->statements.emplace_back(stmt);
        }

        // }
        if (auto diagnostic = parser.expect_or_error(closing, false))
            result.error(std::move(diagnostic));
        else
            block->end_position = &lexer.next()->range.end;

        result.data = std::move(block);
    }

    void Block::recover(Parser &parser)
    {
        using Lexer::TokenType;

        parser.synchronize({TokenType::RightBrace});
    }

} // namespace Parser
