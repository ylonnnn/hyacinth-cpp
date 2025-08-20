#include "core/environment/Environment.hpp"
#include "core/type/compound/Struct.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Expr.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::InstanceExpr>::interpret(Interpreter &interpreter,
                                                  AST::InstanceExpr &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, node.value_ptr(), {}};

        if (result.data != nullptr)
            return result;

        Core::Environment *current = interpreter.current_env();

        result.data = std::make_shared<Core::Value>(Core::object{});
        auto &obj = std::get<Core::object>(*result.data);

        AST::Type &ast_type = node.type();
        Core::BaseType *resolved =
            current->resolve_type(std::string(ast_type.value().value));

        if (resolved == nullptr)
        {
            result.error(Diagnostic::create_unknown_type_error(&ast_type));
            return result;
        }

        Core::TypeResolutionResult t_res = resolved->resolve(ast_type);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        obj.type() = t_res.data;

        auto &fields = node.fields();
        auto bind = true;

        for (auto &[name, field] : fields)
        {
            AST::Expr &f_val = field.value();

            InterpretationResult v_res =
                InterpreterImpl<AST::Expr>::interpret(interpreter, f_val);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            if (v_res.data == nullptr)
            {
                bind = false;
                continue;
            }

            Core::Type *v_type = Core::Type::from_value(current, *v_res.data);
            obj.set(name, Core::value_data{std::move(v_res.data), v_type});
        }

        if (bind)
            node.set_value(result.data);

        return result;
    }

} // namespace Interpreter
