#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/rules/Variable.hpp"

namespace Parser::Hyacinth
{
    void initialize(Grammar &grammar)
    {
        auto variable = std::make_unique<VariableDefinition>();
        grammar.add_rule(VARIABLE, std::move(variable));
    }

} // namespace Parser::Hyacinth
