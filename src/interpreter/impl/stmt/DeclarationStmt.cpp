#include "ast/stmt/types/struct/StructDeclStmt.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Stmt.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::DeclarationStmt>::interpret(Interpreter &interpreter,
                                                     AST::DeclarationStmt &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, nullptr, {}};
        AST::DeclarationStmt *stmt = &node;

        if (auto ptr = dynamic_cast<AST::VariableDeclarationStmt *>(stmt))
            return InterpreterImpl<AST::VariableDeclarationStmt>::interpret(
                interpreter, *ptr);

        else if (auto ptr = dynamic_cast<AST::FunctionDeclarationStmt *>(stmt))
            return InterpreterImpl<AST::FunctionDeclarationStmt>::interpret(
                interpreter, *ptr);

        else if (dynamic_cast<AST::StructDeclarationStmt *>(stmt) != nullptr)
            return result;

        result.status = Core::ResultStatus::Fail;

        return result;
    }

} // namespace Interpreter
