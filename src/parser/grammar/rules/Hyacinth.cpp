#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/rules/Function.hpp"
#include "parser/grammar/rules/Import.hpp"
#include "parser/grammar/rules/Petal.hpp"
#include "parser/grammar/rules/Variable.hpp"
#include "parser/grammar/rules/accessibility/GlobalAccessibility.hpp"
// #include "parser/grammar/rules/types/Struct.hpp"

namespace Parser::Hyacinth
{
    void initialize(Grammar &grammar)
    {
        grammar.add_rule(IMPORT, std::make_unique<ImportStatement>());

        grammar.add_rule(PETAL, std::make_unique<PetalDefinition>());

        // // Global Accessibility
        grammar.add_rule(PUBLIC, std::make_unique<GlobalAccessibility>(PUBLIC));

        // grammar.add_rule(STRUCT, std::make_unique<StructDefinition>(),
        //                  {true, true});

        grammar.add_rule(FUNCTION, std::make_unique<FunctionDefinition>());
        grammar.add_rule(FN_RETURN,
                         std::make_unique<FunctionDefinitionReturn>());

        grammar.add_rule(VARIABLE, std::make_unique<VariableDefinition>());
    }

} // namespace Parser::Hyacinth
