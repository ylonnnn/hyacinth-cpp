#include "interpreter/Interpreter.hpp"
#include "core/result/Result.hpp"

namespace Interpreter
{
    InterpretationResult::InterpretationResult(
        Core::ResultStatus status, const std::shared_ptr<Core::Value> &data,
        Diagnostic::DiagnosticList diagnostics)
        : Core::Result<std::shared_ptr<Core::Value>>(status, data,
                                                     std::move(diagnostics))
    {
    }

    Interpreter::Interpreter(Core::ProgramFile &program)
        : program_(program), environment_(program.environment()),
          current_env_(&environment_)
    {
    }

    Core::ProgramFile &Interpreter::program() { return program_; }

    const Core::ProgramFile &Interpreter::program() const { return program_; }

    Core::Environment &Interpreter::environment() { return environment_; }

    const Core::Environment &Interpreter::environment() const
    {
        return environment_;
    }

    Core::Environment *Interpreter::current_env() { return current_env_; }
    const Core::Environment *Interpreter::current_env() const
    {
        return current_env_;
    };

    void Interpreter::set_current_env(Core::Environment &env)
    {
        current_env_ = &env;
    }

} // namespace Interpreter
