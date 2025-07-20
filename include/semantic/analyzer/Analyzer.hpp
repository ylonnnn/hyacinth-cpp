#pragma once

#include <type_traits>

#include "ast/Node.hpp"
#include "core/environment/Environment.hpp"
#include "core/result/Result.hpp"
#include "core/type/Type.hpp"
#include "core/value/Value.hpp"
#include "diagnostic/Diagnostic.hpp"

namespace Semantic
{
    struct AnalysisResult : public Core::Result<Core::Type *>
    {
        std::optional<Core::Value> value = std::nullopt;

        AnalysisResult(std::optional<Core::Value> value,
                       Core::ResultStatus status, Core::Type *data,
                       Diagnostic::DiagnosticList diagnostics);
    };

    template <typename T,
              typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
    struct AnalyzerImpl
    {
        static AnalysisResult analyze(class Analyzer &analyzer, T &node);
    };

    class Analyzer
    {
      protected:
        Core::ProgramFile &program_;
        Core::Environment environment_;
        // ScopeStack &scope_stack_;

        Core::Environment &current_env_;

      public:
        Analyzer(Core::ProgramFile &program);

        Core::ProgramFile &program();
        Core::Environment &environment();

        Core::Environment &current_env();

        template <typename T,
                  typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
        AnalysisResult analyze(T &node)
        {
            return AnalyzerImpl<T>::analyze(*this, node);
        }
    };

} // namespace Semantic
