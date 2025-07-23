#include "parser/grammar/common/Block.hpp"
#include "ast/stmt/BlockStmt.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    Block::Block(std::pair<Lexer::TokenType, Lexer::TokenType> pair)
        : GrammarRule(pair.first), pair_(std::move(pair))
    {
    }

    ParseResult Block::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        if (lexer.eof(false))
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        auto body = std::make_unique<AST::BlockStmt>(
            lexer.peek()->position, std::vector<std::unique_ptr<AST::Stmt>>{});
        std::vector<std::unique_ptr<AST::Stmt>> &statements =
            body->statements();

        statements.reserve(32);

        result.data = std::move(body);

        if (parser.expect(pair_.first, false))
            lexer.next();
        else
        {
            result.status = Core::ResultStatus::Fail;
            return result;
        }

        while (!parser.expect(pair_.second, false))
        {
            ParseResult p_res = parser.grammar().partial_parse(parser, false);

            auto invalid_data = p_res.data == nullptr;
            if (!invalid_data)
                if (auto ptr = dynamic_cast<AST::Stmt *>(p_res.data.release()))
                    statements.push_back(std::unique_ptr<AST::Stmt>(ptr));

            result.adapt(p_res.status, std::move(p_res.diagnostics));

            if (invalid_data)
            {
                parser.panic();
                break;
            }
        }

        if (auto diagnostic = parser.expect_or_error(pair_.second, false))
        {
            parser.panic();

            result.status = Core::ResultStatus::Fail;
            result.diagnostics.push_back(std::move(diagnostic));
        }

        else
            lexer.next();

        return result;
    }

} // namespace Parser
