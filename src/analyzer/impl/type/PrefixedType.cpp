#include "analyzer/impl/Type.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::PrefixedType>::analyze(Analyzer &analyzer,
                                             AST::PrefixedType &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("impement AST::PrefixedType analyzer");

        return result;
    }

} // namespace Semantic
