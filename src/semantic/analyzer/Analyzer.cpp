#include "semantic/analyzer/Analyzer.hpp"
#include "core/type/primitive/Integer.hpp"

namespace Semantic
{
    AnalysisResult::AnalysisResult(std::optional<Core::Value> value,
                                   Core::ResultStatus status, Core::Type *data,
                                   Diagnostic::DiagnosticList diagnostics)
        : Core::Result<Core::Type *>(status, data, std::move(diagnostics)),
          value(std::move(value))
    {
    }

    Analyzer::Analyzer(Core::ProgramFile &program)
        : program_(program), environment_(nullptr), current_env_(environment_)
    {
        current_env_.declare_type(
            std::make_unique<Core::IntegerType>(&current_env_, true));
        current_env_.declare_type(
            std::make_unique<Core::IntegerType>(&current_env_, false));
    }

    Core::ProgramFile &Analyzer::program() { return program_; }

    Core::Environment &Analyzer::environment() { return environment_; }

    Core::Environment &Analyzer::current_env() { return current_env_; }

} // namespace Semantic
