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
            AnalysisResult gls_res =
                AnalyzerImpl<AST::GlobalStmt>::analyze(analyzer, *statement);

            result.adapt(gls_res.status, std::move(gls_res.diagnostics));
        }

        return result;
    }

} // namespace Semantic
