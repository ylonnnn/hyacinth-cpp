#include "parser/grammar/rules/Petal.hpp"
#include "ast/stmt/BlockStmt.hpp"
#include "ast/stmt/petal/PetalDefStmt.hpp"
#include "parser/grammar/GrammarContext.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/dev.hpp"
#include "utils/pointer.hpp"

namespace Parser
{
    PetalDefinition::PetalDefinition() : GrammarRule(Hyacinth::PETAL, GC_GLOBAL)
    {
    }

    ParseResult PetalDefinition::parse(Parser &parser)
    {
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void PetalDefinition::parse(Parser &parser, ParseResult &result)
    {
        using Lexer::TokenType;

        // petal PATH ( BLOCK | ";" )
        auto &lexer = parser.lexer;

        // petal
        lexer.consume();

        // PATH
        ParseResult p_res = Common::PathRule.parse(parser);
        result.adapt(p_res.status, std::move(p_res.diagnostics));

        auto identifier = utils::dynamic_ptr_cast<AST::Path>(p_res.data);
        if (identifier == nullptr)
        {
            utils::todo(
                "throw error: petal identifier must be a valid identifier "
                "or path");
            return;
        }

        // BLOCK
        if (parser.expect(TokenType::LeftBrace, false))
        {
            ParseResult b_res{parser, Core::ResultStatus::Success, nullptr, {}};
            Common::Block.parse(parser, GC_GLOBAL, b_res);

            result.adapt(b_res.status, std::move(b_res.diagnostics));
            auto block = utils::dynamic_ptr_cast<AST::BlockStmt>(b_res.data);

            if (block == nullptr)
            {
                utils::todo(
                    "throw error(?): block must be a block of statements");
                return;
            }

            result.data = std::make_unique<AST::PetalDefinitionStmt>(
                std::move(identifier), std::move(block));

            return;
        }

        // ;
        Common::Terminator.parse(parser, result);
        result.data =
            std::make_unique<AST::PetalDeclarationStmt>(std::move(identifier));
    }

    void PetalDefinition::recover(Parser &parser)
    {
        using Lexer::TokenType;

        parser.synchronize({TokenType::Semicolon, TokenType::RightBrace});
    }

} // namespace Parser
