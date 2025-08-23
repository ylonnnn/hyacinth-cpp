#include "interpreter/impl/Block.hpp"
#include "interpreter/Interpreter.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::Block>::interpret(Interpreter &interpreter,
                                           AST::Block &node)
    {
        AST::Block *block = &node;

        if (auto ptr = dynamic_cast<AST::GlobalBlock *>(block))
            return interpreter.interpret(*ptr);

        return {Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Interpreter
