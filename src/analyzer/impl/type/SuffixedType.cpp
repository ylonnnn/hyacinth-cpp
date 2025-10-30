#include "analyzer/impl/Type.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::SuffixedType>::analyze(Analyzer &analyzer,
                                             AST::SuffixedType &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("impement AST::SuffixedType analyzer");

        return result;
    }

} // namespace Semantic
