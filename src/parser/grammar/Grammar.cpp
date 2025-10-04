#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "ast/GlobalNode.hpp"
#include "diagnostic/Diagnostic.hpp"
#include "diagnostic/helpers.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/GrammarContext.hpp"
#include "parser/grammar/common/expr/Expr.hpp"

namespace Parser
{
    Grammar::Grammar() : fallback(std::make_unique<Expr>())
    {
        rules.reserve(16);
    }

    void Grammar::add_rule(Lexer::TokenType type,
                           std::unique_ptr<GrammarRule> rule)
    {
    }

    GrammarRule *Grammar::get_rule(Lexer::TokenType type,
                                   GrammarContext context) const
    {
        auto it = rules.find(type);
        if (it == rules.end())
            return nullptr;

        return it->second.get();
    }

    ParseResult Grammar::partial_parse(Parser &parser, GrammarContext context)
    {
        Lexer::Lexer &lexer = parser.lexer;
        ParseResult result{parser, Core::ResultStatus::Success, nullptr, {}};

        result.diagnostics.reserve(32);

        if (lexer.eof(false))
            return result;

        Lexer::Token *token = lexer.peek();
        if (token == nullptr)
            return result;

        GrammarRule *rule = get_rule(token->type, context);
        if (rule == nullptr)
        {
            std::cout << (fallback->context & context) << "\n";
            if (fallback->context & context)
                rule = fallback.get();

            else
            {
                result.error(Diagnostic::create_syntax_error(*token));
                lexer.consume();

                return result;
            }
        }

        ParseResult p_res = rule->parse(parser);
        result.adapt(p_res.status, std::move(p_res.diagnostics),
                     std::move(p_res.data));

        return result;
    }

    ProgramParseResult Grammar::parse(Parser &parser)
    {
        Lexer::Lexer &lexer = parser.lexer;
        ProgramParseResult result(
            parser, Core::ResultStatus::Success,
            std::make_unique<AST::Program>(parser.program), {});

        result.diagnostics.reserve(64);

        while (!lexer.eof(false))
        {
            ParseResult p_res = partial_parse(parser, GC_GLOBAL);
            result.adapt(p_res.status, std::move(p_res.diagnostics));

            if (p_res.data == nullptr)
                continue;

            if (auto ptr = dynamic_cast<AST::GlobalNode *>(p_res.data.get()))
            {
                result.data->nodes.emplace_back(ptr);
                auto _ = p_res.data.release();
                (void)_;
            }
        }

        return result;
    }

} // namespace Parser
