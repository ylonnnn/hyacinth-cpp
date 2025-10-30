#pragma once

#include "analyzer/Analyzer.hpp"
#include "ast/common/Identifier.hpp"

namespace Semantic
{
    template <> struct AnalyzerImpl<AST::Identifier>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::Identifier &node);
    };

} // namespace Semantic
