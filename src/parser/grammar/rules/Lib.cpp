#include "parser/grammar/rules/Lib.hpp"
#include "ast/block/Block.hpp"
#include "ast/block/LibBlock.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/BlockStmt.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    LibBlock::LibBlock() : GrammarRule(Hyacinth::LIB) {}

    ParseResult LibBlock::parse(Parser &parser)
    {
        // lib IDENTIFIER { ... }
        // lib IDENTIFIER::IDENTIFIER1::... { ... }

        using namespace Lexer::TokenTypes;
        using namespace Operator;

        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        // lib (KeywordLib)
        if (auto diagnostic = parser.expect_or_error(token_type_, false))
            result.force_error(std::move(diagnostic));
        else
            lexer.next();

        // IDENTIFIER
        std::unique_ptr<AST::LibBlock> lib;
        AST::LibBlock *curr_lib = nullptr;

        do
        {
            Lexer::Token *target = nullptr;
            if (parser.expect(Primary::Identifier, false))
                target = lexer.next();
            else
            {
                auto node = AST::LiteralExpr(*lexer.peek());
                result.force_error(
                    &node, Diagnostic::ErrorTypes::Syntax::MissingImportTarget,
                    std::string("Missing ") + Diagnostic::ERR_GEN +
                        "LIBRARY NAME" + Utils::Styles::Reset + ".",
                    "Missing library name here");
            }

            auto node = std::make_unique<AST::LibBlock>(*target);

            if (lib == nullptr)
            {
                lib = std::move(node);
                curr_lib = lib.get();

                continue;
            }

            std::vector<std::unique_ptr<AST::Stmt>> &statements =
                curr_lib->statements();

            curr_lib = node.get();
            statements.push_back(
                std::make_unique<AST::BlockStmt>(std::move(node)));

        } while ((lexer.peek()->type == Lexer::TokenType{Access::DoubleColon}
                      ? lexer.next() != nullptr
                      : false));

        // { ... }
        ParseResult b_res = Common::Block.parse(parser, true);
        result.adapt(b_res.status, std::move(b_res.diagnostics));

        auto block = dynamic_cast<AST::Block *>(b_res.data.get());
        curr_lib->statements().swap(block->statements());

        result.data = std::move(lib);

        return result;
    }

} // namespace Parser
