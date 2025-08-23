#pragma once

#include "ast/block/Block.hpp"
#include "ast/block/GlobalBlock.hpp"
#include "ast/block/LibBlock.hpp"
#include "interpreter/Interpreter.hpp"

namespace Interpreter
{
    template <> struct InterpreterImpl<AST::Block>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::Block &node);
    };

    template <> struct InterpreterImpl<AST::GlobalBlock>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::GlobalBlock &node);
    };

    template <> struct InterpreterImpl<AST::LibBlock>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::LibBlock &node);
    };

} // namespace Interpreter
