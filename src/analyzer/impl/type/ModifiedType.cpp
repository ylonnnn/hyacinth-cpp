#include "analyzer/impl/Type.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::ModifiedType>::analyze(Analyzer &analyzer,
                                             AST::ModifiedType &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("impement AST::ModifiedType analyzer");

        return result;
    }

} // namespace Semantic
