#include "interpreter/impl/Stmt.hpp"
#include "interpreter/Interpreter.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::Stmt>::interpret(Interpreter &interpreter,
                                          AST::Stmt &node)
    {
        AST::Stmt *stmt = &node;

        if (auto ptr = dynamic_cast<AST::GlobalStmt *>(stmt))
            return InterpreterImpl<AST::GlobalStmt>::interpret(interpreter,
                                                               *ptr);

        else if (typeid(*stmt) == typeid(AST::FunctionReturnStmt))
            return InterpreterImpl<AST::FunctionReturnStmt>::interpret(
                interpreter, *static_cast<AST::FunctionReturnStmt *>(stmt));

        else if (typeid(*stmt) == typeid(AST::ExprStmt))
            return InterpreterImpl<AST::ExprStmt>::interpret(
                interpreter, *static_cast<AST::ExprStmt *>(stmt));

        return {Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Interpreter
