#include "interpreter/impl/Stmt.hpp"
#include "interpreter/Interpreter.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::Stmt>::interpret(Interpreter &interpreter,
                                          AST::Stmt &node)
    {
        if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(&node))
            return interpreter.interpret(*ptr);

        else if (typeid(node) == typeid(AST::FunctionReturnStmt))
            return interpreter.interpret(
                static_cast<AST::FunctionReturnStmt &>(node));

        else if (typeid(node) == typeid(AST::ExprStmt))
            return interpreter.interpret(static_cast<AST::ExprStmt &>(node));

        else if (typeid(node) == typeid(AST::BlockStmt))
            return interpreter.interpret(static_cast<AST::BlockStmt &>(node));

        return {Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Interpreter
