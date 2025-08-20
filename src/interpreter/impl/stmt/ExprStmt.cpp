#include "interpreter/impl/Stmt.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::ExprStmt>::interpret(Interpreter &interpreter,
                                              AST::ExprStmt &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, node.value_ptr(), {}};

        if (result.data != nullptr)
            return result;

        InterpretationResult e_res =
            InterpreterImpl<AST::Expr>::interpret(interpreter, node.expr());
        result.adapt(e_res.status, std::move(e_res.diagnostics), e_res.data);

        return result;
    }

} // namespace Interpreter
