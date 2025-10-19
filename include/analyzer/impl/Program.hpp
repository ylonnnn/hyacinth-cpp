#pragma once

#include "analyzer/Analyzer.hpp"
#include "ast/Program.hpp"

namespace Semantic
{
    template <> struct AnalyzerImpl<AST::Program>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::Program &node);
    };

} // namespace Semantic
