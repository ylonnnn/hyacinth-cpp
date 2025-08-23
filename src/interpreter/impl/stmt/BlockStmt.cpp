#include "ast/stmt/BlockStmt.hpp"
#include "interpreter/impl/Stmt.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::BlockStmt>::interpret(Interpreter &interpreter,
                                               AST::BlockStmt &node)
    {
        return interpreter.interpret(node.block());
    }

} // namespace Interpreter
