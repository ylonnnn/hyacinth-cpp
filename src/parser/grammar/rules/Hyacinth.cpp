#include "parser/grammar/rules/Hyacinth.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/rules/Variable.hpp"
// #include "parser/grammar/rules/Function.hpp"
// #include "parser/grammar/rules/Import.hpp"
// #include "parser/grammar/rules/Lib.hpp"
// #include "parser/grammar/rules/Variable.hpp"
// #include "parser/grammar/rules/accessibility/GlobalAccessibility.hpp"
// #include "parser/grammar/rules/types/Struct.hpp"

namespace Parser::Hyacinth
{
    void initialize(Grammar &grammar)
    {
        grammar.add_rule(VARIABLE, std::make_unique<VariableDefinition>());

        // grammar.add_rule(IMPORT, std::make_unique<ImportStatement>(),
        //                  {true, false});

        // grammar.add_rule(LIB, std::make_unique<LibBlock>(), {true, false});

        // // Global Accessibility
        // grammar.add_rule(PUBLIC,
        // std::make_unique<GlobalAccessibility>(PUBLIC),
        //                  {true, false});
        // grammar.add_rule(PRIVATE,
        //                  std::make_unique<GlobalAccessibility>(PRIVATE),
        //                  {true, false});

        // grammar.add_rule(STRUCT, std::make_unique<StructDefinition>(),
        //                  {true, true});

        // grammar.add_rule(FUNCTION, std::make_unique<FunctionDefinition>(),
        //                  {true, true});
        // grammar.add_rule(Lexer::TokenTypes::Reserved::Return,
        //                  std::make_unique<FunctionDefinitionReturn>(),
        //                  {false, true});

        // grammar.add_rule(VARIABLE, std::make_unique<VariableDefinition>(),
        //                  {true, true});
    }

} // namespace Parser::Hyacinth
