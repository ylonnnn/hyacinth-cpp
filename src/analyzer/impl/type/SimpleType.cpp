#include "analyzer/impl/Type.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::SimpleType>::analyze(Analyzer &analyzer,
                                                          AST::SimpleType &node)
    {
        return AnalyzerImpl<AST::Path>::analyze(analyzer, *node.base);
    }

} // namespace Semantic
