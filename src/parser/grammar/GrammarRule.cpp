#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    GrammarRule::GrammarRule(Lexer::TokenType token_type)
        : token_type(token_type)
    {
    }

} // namespace Parser
