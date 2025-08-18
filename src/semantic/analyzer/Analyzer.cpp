#include "semantic/analyzer/Analyzer.hpp"
#include "core/environment/Environment.hpp"
#include "core/type/compound/Array.hpp"
#include "core/type/primitive/Boolean.hpp"
#include "core/type/primitive/Character.hpp"
#include "core/type/primitive/Float.hpp"
#include "core/type/primitive/Integer.hpp"
#include "core/type/primitive/String.hpp"
#include "core/type/primitive/Void.hpp"

namespace Semantic
{
    AnalysisResult::AnalysisResult(std::shared_ptr<Core::Value> value,
                                   Core::ResultStatus status, Core::Type *data,
                                   Diagnostic::DiagnosticList diagnostics)
        : Core::Result<Core::Type *>(status, data, std::move(diagnostics)),
          value(std::move(value))
    {
    }

    Analyzer::Analyzer(Core::ProgramFile &program)
        : program_(program), environment_(program_.environment()),
          current_env_(&environment_)
    {
        initialize_types();
    }

    void Analyzer::initialize_types()
    {
        // Return Type Only
        environment_.declare_type(std::make_unique<Core::Void>(&environment_));

        environment_.declare_type(
            std::make_unique<Core::ArrayType>(&environment_));

        environment_.declare_type(
            std::make_unique<Core::BooleanType>(&environment_));

        environment_.declare_type(
            std::make_unique<Core::CharacterType>(&environment_));

        environment_.declare_type(
            std::make_unique<Core::IntegerType>(&environment_, true));
        environment_.declare_type(
            std::make_unique<Core::IntegerType>(&environment_, false));

        environment_.declare_type(
            std::make_unique<Core::FloatType>(&environment_));

        environment_.declare_type(
            std::make_unique<Core::StringType>(&environment_));
    }

    Core::ProgramFile &Analyzer::program() { return program_; }

    Core::Environment &Analyzer::environment() { return environment_; }

    Core::Environment *Analyzer::current_env() { return current_env_; }

    void Analyzer::set_current_env(Core::Environment &env)
    {
        current_env_ = &env;
    }

} // namespace Semantic
