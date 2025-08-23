#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Block.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::GlobalBlock>::interpret(Interpreter &interpreter,
                                                 AST::GlobalBlock &node)
    {
        if (typeid(node) == typeid(AST::LibBlock))
            return interpreter.interpret(*static_cast<AST::LibBlock *>(&node));

        return {Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Interpreter
