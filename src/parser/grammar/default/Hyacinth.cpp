#include "parser/grammar/default/Hyacinth.hpp"
#include "parser/grammar/default/Classes.hpp"
#include "parser/grammar/default/Functions.hpp"

namespace Parser::Hyacinth
{
    void initialize(Grammar *grammar)
    {
        grammar->make_rule_set<FunctionDefinition>(
            Lexer::TokenTypes::Reserved::Function);

        grammar->make_rule_set<ClassDefinition>(
            Lexer::TokenTypes::Reserved::Class);
    }

} // namespace Parser::Hyacinth
