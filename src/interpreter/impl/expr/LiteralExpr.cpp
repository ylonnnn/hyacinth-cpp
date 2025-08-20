#include "interpreter/impl/Expr.hpp"
#include "utils/value.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::LiteralExpr>::interpret(Interpreter &interpreter,
                                                 AST::LiteralExpr &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, node.value_ptr(), {}};

        if (result.data != nullptr)
            return result;

        result.data =
            std::make_shared<Core::Value>(Utils::parse_val(node.value()));

        return result;
    }

} // namespace Interpreter
