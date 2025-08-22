#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "core/environment/Environment.hpp"
#include "core/symbol/FunctionSymbol.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Stmt.hpp"

namespace Interpreter
{
    static void interpret_body(Interpreter &interpreter,
                               Core::FunctionSymbol &fn,
                               InterpretationResult &result)
    {
        if (fn.node == nullptr)
            return;

        auto &stmt = static_cast<AST::FunctionDefinitionStmt &>(*fn.node);
        AST::Block &body = stmt.body();

        for (const auto &statement : body.statements())
        {
            AST::Stmt &stmt = *statement;
            InterpretationResult si_res = interpreter.interpret(stmt);
            result.adapt(si_res.status, std::move(si_res.diagnostics));

            if (typeid(stmt) == typeid(AST::FunctionReturnStmt))
                result.data = si_res.data;
        }
    }

    InterpretationResult
    InterpreterImpl<AST::FunctionDeclarationStmt>::interpret(
        Interpreter &interpreter, AST::FunctionDeclarationStmt &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, nullptr, {}};
        Core::Environment *current = interpreter.current_env();

        auto function = static_cast<Core::FunctionSymbol *>(
            current->resolve_symbol(std::string(node.name().value)));

        if (node.is_definition())
        {
            interpreter.set_current_env(*function->environment);
            interpret_body(interpreter, *function, result);
            interpreter.set_current_env(*current);
        }

        return result;
    }

} // namespace Interpreter
