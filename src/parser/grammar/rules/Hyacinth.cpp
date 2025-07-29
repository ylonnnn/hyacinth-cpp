#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/rules/Function.hpp"
#include "parser/grammar/rules/Variable.hpp"
#include "parser/grammar/rules/types/Struct.hpp"

namespace Parser::Hyacinth
{
    void initialize(Grammar &grammar)
    {
        grammar.add_rule(STRUCT, std::make_unique<StructDefinition>());

        grammar.add_rule(FUNCTION, std::make_unique<FunctionDefinition>());
        grammar.add_rule(Lexer::TokenTypes::Reserved::Return,
                         std::make_unique<FunctionDefinitionReturn>(), false);

        grammar.add_rule(VARIABLE, std::make_unique<VariableDefinition>());
    }

} // namespace Parser::Hyacinth
