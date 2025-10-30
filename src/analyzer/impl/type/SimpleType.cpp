#include "analyzer/impl/Type.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::SimpleType>::analyze(Analyzer &analyzer,
                                                          AST::SimpleType &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("impement AST::SimpleType analyzer");

        AnalysisResult b_res =
            AnalyzerImpl<AST::Path>::analyze(analyzer, *node.base);

        return result;
    }

} // namespace Semantic
