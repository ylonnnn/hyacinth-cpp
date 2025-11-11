#include "analyzer/Analyzer.hpp"
#include "core/environment/Environment.hpp"
#include "core/symbol/TypeSymbol.hpp"
#include "core/type/Type.hpp"
#include "core/type/builtin/Char.hpp"
#include "core/type/builtin/Str.hpp"
#include "core/type/builtin/numeric/Float.hpp"
#include "core/type/builtin/numeric/Integer.hpp"
#include "core/type/wrapper/ArrayType.hpp"
#include "core/type/wrapper/ReferenceType.hpp"
// #include "core/type/compound/Array.hpp"
// #include "core/type/primitive/Boolean.hpp"
// #include "core/type/primitive/Character.hpp"
// #include "core/type/primitive/Float.hpp"
// #include "core/type/primitive/Integer.hpp"
// #include "core/type/primitive/String.hpp"
// #include "core/type/primitive/Void.hpp"

namespace Semantic
{
    AnalysisResult::AnalysisResult(Core::Value *value,
                                   Core::ResultStatus status,
                                   Core::InstantiatedType *data,
                                   Diagnostic::DiagnosticList diagnostics)
        : Core::Result<Core::InstantiatedType *>(status, data,
                                                 std::move(diagnostics)),
          value(value)
    {
    }

    void AnalysisResult::adapt(AnalysisResult &result)
    {
        Core::Result<Core::InstantiatedType *>::adapt(
            result.status, std::move(result.diagnostics), result.data);

        value = result.value;
        symbol = result.symbol;
    }

    Analyzer::Analyzer(Core::Program &program) : program(program)
    {
        initialize();
    }

    void Analyzer::initialize()
    {
        env_stack.enter(*program.environment);

        initialize_types();
    }

    void Analyzer::initialize_types()
    {
        Core::Environment *root = env_stack.root();

        // Initialize Type Wrappers (e.g. []T, *T, &T, T?, etc)
        Core::ArrayType::instance(root);
        Core::ReferenceType::instance(root);

        auto add_type = [root](std::unique_ptr<Core::BaseType> &&type) -> void
        {
            root->add_symbol(std::make_unique<Core::TypeSymbol>(
                type->name, nullptr, Core::SymbolAccessibility::Public, *type));
            root->add_type(std::move(type));
        };

        // int<{}>, uint<{}>
        add_type(std::make_unique<Core::IntegerType>(*root, true));
        add_type(std::make_unique<Core::IntegerType>(*root, false));

        // float<{}>
        // add_type(std::make_unique<Core::FloatType>(*root));

        // char
        // add_type(std::make_unique<Core::CharType>(*root));

        // str
        // add_type(std::make_unique<Core::StrType>(*root));
    }

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
    // }

} // namespace Semantic
