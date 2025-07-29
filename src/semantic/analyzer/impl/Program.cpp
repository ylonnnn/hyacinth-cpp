#include "semantic/analyzer/impl/Program.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Program>::analyze(Analyzer &analyzer,
                                                       AST::Program &node)
    {
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};
        result.diagnostics.reserve(32);

        for (auto &declaration : node.declarations())
        {
            AnalysisResult decl_result =
                AnalyzerImpl<std::remove_cv_t<std::remove_reference_t<
                    decltype(*declaration)>>>::analyze(analyzer, *declaration);

            result.adapt(decl_result.status,
                         std::move(decl_result.diagnostics));
        }

        return result;
    }

} // namespace Semantic
