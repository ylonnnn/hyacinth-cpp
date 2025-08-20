#include "core/environment/Environment.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Expr.hpp"

namespace Interpreter
{
    static void interpret_fn_call(Interpreter &interpreter,
                                  AST::FunctionCallExpr &node,
                                  InterpretationResult &result)
    {
        Core::Environment *initial = interpreter.current_env();
        Core::FunctionSymbol *fn = node.fn_symbol();

        interpreter.set_current_env(*fn->environment);
        Core::Environment *current = interpreter.current_env();

        // Substitute arguments to its corresponding parameters
        std::vector<Core::FunctionParameter> &parameters = fn->parameters;
        std::vector<std::unique_ptr<AST::Expr>> &arguments = node.arguments();

        std::vector<std::pair<Core::FunctionParameterSymbol *,
                              std::shared_ptr<Core::Value>>>
            p_initial(parameters.size());

        for (size_t i = 0; i < parameters.size(); i++)
        {
            auto &parameter = parameters[i];
            auto &argument = arguments[i];

            std::shared_ptr<Core::Value> arg_val = argument->value_ptr();
            if (arg_val == nullptr)
            {
                InterpretationResult ari_res = interpreter.interpret(*argument);
                result.adapt(ari_res.status, std::move(ari_res.diagnostics),
                             ari_res.data);

                arg_val = ari_res.data;
            }

            auto param_sym = static_cast<Core::FunctionParameterSymbol *>(
                current->resolve_symbol(
                    std::string(parameter.name),
                    static_cast<size_t>(Core::ResolutionType::Current)));

            p_initial[i] = {param_sym, param_sym->value};
            param_sym->value = arg_val;
        }

        // Interpret the statements of the function
        InterpretationResult i_res = interpreter.interpret(*fn->node);
        result.adapt(i_res.status, std::move(i_res.diagnostics), i_res.data);

        for (auto &[param, value] : p_initial)
            param->value = value;

        interpreter.set_current_env(*initial);
    }

    InterpretationResult InterpreterImpl<AST::FunctionCallExpr>::interpret(
        Interpreter &interpreter, AST::FunctionCallExpr &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, nullptr, {}};

        interpret_fn_call(interpreter, node, result);

        return result;
    }
} // namespace Interpreter
