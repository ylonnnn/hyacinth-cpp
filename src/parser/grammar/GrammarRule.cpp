#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    GrammarRule::GrammarRule(Lexer::TokenType token_type)
        : token_type_(token_type)
    {
    }

    Lexer::TokenType GrammarRule::token_type() const { return token_type_; }

} // namespace Parser
