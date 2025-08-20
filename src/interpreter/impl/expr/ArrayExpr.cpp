#include "core/value/Value.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Expr.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::ArrayExpr>::interpret(Interpreter &interpreter,
                                               AST::ArrayExpr &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, node.value_ptr(), {}};

        if (result.data != nullptr)
            return result;

        Core::Environment *current = interpreter.current_env();

        result.data = std::make_shared<Core::Value>(Core::array{nullptr});
        auto &arr = std::get<Core::array>(*result.data);

        Core::Type *&el_type = arr.element_type();
        auto bind = true;

        for (auto &element : node.elements())
        {
            InterpretationResult a_res =
                InterpreterImpl<AST::Expr>::interpret(interpreter, *element);
            result.adapt(a_res.status, std::move(a_res.diagnostics));

            Core::Type *v_type = Core::Type::from_value(current, *a_res.data);

            el_type =
                el_type == nullptr
                    ? v_type
                    : (el_type->assignable_with(*v_type)
                           ? el_type
                           : (v_type->assignable_with(*el_type) ? v_type
                                                                : el_type));

            if (el_type == nullptr)
                break;

            if (a_res.data == nullptr)
                bind = false;

            arr.elements().emplace_back(
                Core::value_data{std::move(a_res.data), v_type});
        }

        if (bind)
            node.set_value(result.data);

        return result;
    }

} // namespace Interpreter
