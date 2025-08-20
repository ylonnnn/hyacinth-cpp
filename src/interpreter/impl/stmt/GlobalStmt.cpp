#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Stmt.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::GlobalStmt>::interpret(Interpreter &interpreterr,
                                                AST::GlobalStmt &node)
    {
        if (typeid(node) == typeid(AST::ImportStmt))
            return InterpreterImpl<AST::ImportStmt>::interpret(
                interpreterr, static_cast<AST::ImportStmt &>(node));

        else if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(&node))
            return InterpreterImpl<AST::DeclarationStmt>::interpret(
                interpreterr, *ptr);

        return {Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Interpreter
