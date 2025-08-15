#include "semantic/analyzer/impl/Program.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Program>::analyze(Analyzer &analyzer,
                                                       AST::Program &node)
    {
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};
        result.diagnostics.reserve(32);

        for (auto &statement : node.statements())
        {
            AnalysisResult decl_result =
                AnalyzerImpl<std::remove_cv_t<std::remove_reference_t<
                    decltype(*statement)>>>::analyze(analyzer, *statement);

            result.adapt(decl_result.status,
                         std::move(decl_result.diagnostics));
        }

        return result;
    }

} // namespace Semantic
