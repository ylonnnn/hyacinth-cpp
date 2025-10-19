#include "analyzer/impl/Program.hpp"
#include "analyzer/Analyzer.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Program>::analyze(Analyzer &analyzer,
                                                       AST::Program &node)
    {
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};
        result.diagnostics.reserve(32);

        utils::todo("implement program node/statement analyzer");
        // for (auto &node : node.nodes()) {
        //     AnalysisResult gls_res =
        //         AnalyzerImpl<AST::GlobalNode>::analyze(analyzer, *node);

        //     result.adapt(gls_res.status, std::move(gls_res.diagnostics));
        // }

        return result;
    }

} // namespace Semantic
