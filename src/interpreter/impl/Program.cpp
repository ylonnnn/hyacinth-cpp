#include "interpreter/impl/Program.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::Program>::interpret(Interpreter &interpreter,
                                             AST::Program &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, nullptr, {}};

        for (const auto &statement : node.statements())
        {
            InterpretationResult gls_res =
                InterpreterImpl<AST::GlobalStmt>::interpret(interpreter,
                                                            *statement);

            result.adapt(gls_res.status, std::move(gls_res.diagnostics));
        }

        return result;
    }

} // namespace Interpreter
