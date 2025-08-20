#include "ast/stmt/variable/VariableDefStmt.hpp"
#include "core/environment/Environment.hpp"
#include "core/symbol/VariableSymbol.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Stmt.hpp"

namespace Interpreter
{
    static void interpret_value(Interpreter &interpreter,
                                Core::VariableSymbol &var,
                                InterpretationResult &result)
    {
        if (var.node == nullptr)
            return;

        auto &stmt = static_cast<AST::VariableDefinitionStmt &>(*var.node);

        InterpretationResult i_res = interpreter.interpret(stmt.value());
        result.adapt(i_res.status, std::move(i_res.diagnostics), i_res.data);

        var.value = result.data;

        var.node->set_value(var.value);
    }

    InterpretationResult
    InterpreterImpl<AST::VariableDeclarationStmt>::interpret(
        Interpreter &interpreter, AST::VariableDeclarationStmt &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, node.value_ptr(), {}};
        Core::Environment *current = interpreter.current_env();

        if (result.data != nullptr)
            return result;

        auto variable = static_cast<Core::VariableSymbol *>(
            current->resolve_symbol(std::string(node.name().value)));

        if (node.is_definition())
            interpret_value(interpreter, *variable, result);

        return result;
    }

} // namespace Interpreter
