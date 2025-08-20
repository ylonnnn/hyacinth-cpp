#pragma once

#include "ast/Program.hpp"
#include "interpreter/Interpreter.hpp"

namespace Interpreter
{
    template <> struct InterpreterImpl<AST::Program>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::Program &node);
    };

} // namespace Interpreter
