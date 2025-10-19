#include "analyzer/Analyzer.hpp"
#include "core/environment/Environment.hpp"
// #include "core/type/compound/Array.hpp"
// #include "core/type/primitive/Boolean.hpp"
// #include "core/type/primitive/Character.hpp"
// #include "core/type/primitive/Float.hpp"
// #include "core/type/primitive/Integer.hpp"
// #include "core/type/primitive/String.hpp"
// #include "core/type/primitive/Void.hpp"

namespace Semantic
{
    AnalysisResult::AnalysisResult(std::shared_ptr<Core::Value> value,
                                   Core::ResultStatus status, void *data,
                                   Diagnostic::DiagnosticList diagnostics)
        : Core::Result<void *>(status, data, std::move(diagnostics)),
          value(std::move(value))
    {
    }

    Analyzer::Analyzer(Core::Program &program) : program(program)
    {
        initialize_types();
    }

    void Analyzer::initialize_types()
    {
        // Return Type Only
        // environment_.declare_type(std::make_unique<Core::Void>(&environment_));

        // environment_.declare_type(
        //     std::make_unique<Core::ArrayType>(&environment_));

        // environment_.declare_type(
        //     std::make_unique<Core::BooleanType>(&environment_));

        // environment_.declare_type(
        //     std::make_unique<Core::CharacterType>(&environment_));

        // environment_.declare_type(
        //     std::make_unique<Core::IntegerType>(&environment_, true));
        // environment_.declare_type(
        //     std::make_unique<Core::IntegerType>(&environment_, false));

        // environment_.declare_type(
        //     std::make_unique<Core::FloatType>(&environment_));

        // environment_.declare_type(
        //     std::make_unique<Core::StringType>(&environment_));
    }

} // namespace Semantic
