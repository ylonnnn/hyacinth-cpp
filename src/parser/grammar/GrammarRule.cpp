#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    GrammarRule::GrammarRule(Lexer::TokenType token_type,
                             GrammarContext context)
        : token_type(token_type)
    {
    }

} // namespace Parser
