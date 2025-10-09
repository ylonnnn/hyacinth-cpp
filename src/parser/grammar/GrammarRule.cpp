#include "parser/grammar/GrammarRule.hpp"
#include "parser/Parser.hpp"

namespace Parser
{
    GrammarRule::GrammarRule(Lexer::TokenType token_type,
                             GrammarContext context)
        : token_type(token_type), context(context)
    {
    }

    void GrammarRule::recover(Parser &parser) { parser.synchronize(); }

} // namespace Parser
