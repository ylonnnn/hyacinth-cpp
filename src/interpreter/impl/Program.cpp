#include "interpreter/impl/Program.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::Program>::interpret(Interpreter &interpreter,
                                             AST::Program &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, nullptr, {}};

        for (const auto &node : node.nodes())
        {
            InterpretationResult gls_res =
                InterpreterImpl<AST::GlobalNode>::interpret(interpreter, *node);

            result.adapt(gls_res.status, std::move(gls_res.diagnostics));
        }

        return result;
    }

} // namespace Interpreter
