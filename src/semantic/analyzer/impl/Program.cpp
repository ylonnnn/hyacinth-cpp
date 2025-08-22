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

        for (auto &node : node.nodes())
        {
            AnalysisResult gls_res =
                AnalyzerImpl<AST::GlobalNode>::analyze(analyzer, *node);

            result.adapt(gls_res.status, std::move(gls_res.diagnostics));
        }

        return result;
    }

} // namespace Semantic
