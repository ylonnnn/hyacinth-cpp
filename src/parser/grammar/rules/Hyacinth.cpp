#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/rules/Function.hpp"
#include "parser/grammar/rules/Variable.hpp"

namespace Parser::Hyacinth
{
    void initialize(Grammar &grammar)
    {
        grammar.add_rule(VARIABLE, std::make_unique<VariableDefinition>());

        grammar.add_rule(FUNCTION, std::make_unique<FunctionDefinition>());
    }

} // namespace Parser::Hyacinth
