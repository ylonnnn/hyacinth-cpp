#pragma once

#include <functional>
#include <type_traits>

#include "ast/Node.hpp"
#include "core/environment/EnvironmentStack.hpp"
#include "core/program/Program.hpp"
#include "core/result/Result.hpp"
// #include "core/type/Type.hpp"
// #include "core/value/Value.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"
#include "diagnostic/Diagnostic.hpp"

#define DISPATCH(T, TN)                                                        \
    {typeid(T), [](Analyzer &analyzer, TN &node)                               \
     { return AnalyzerImpl<T>::analyze(analyzer, static_cast<T &>(node)); }}

namespace Semantic
{
    struct AnalysisResult : Core::Result<Core::InstantiatedType *>
    {
        // For Constant Folding
        Core::Value *value = nullptr;
        Core::Symbol *symbol = nullptr;

        AnalysisResult(Core::Value *value, Core::ResultStatus status,
                       Core::InstantiatedType *data,
                       Diagnostic::DiagnosticList diagnostics);

        using Core::Result<Core::InstantiatedType *>::adapt;
        void adapt(AnalysisResult &result);
    };

    template <typename T,
              typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
    using AnalyzerImplFn =
        std::function<AnalysisResult(class Analyzer &analyzer, T &node)>;

    template <typename T,
              typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
    struct AnalyzerImpl
    {
        static AnalysisResult analyze(class Analyzer &analyzer, T &node);
    };

    struct Analyzer
    {
        Core::Program &program;
        Core::EnvironmentStack env_stack;

        Analyzer(Core::Program &program);

        void initialize();
        void initialize_types();

        template <typename T,
                  typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
        AnalysisResult analyze(T &node)
        {
            return AnalyzerImpl<T>::analyze(*this, node);
        }
    };

} // namespace Semantic
