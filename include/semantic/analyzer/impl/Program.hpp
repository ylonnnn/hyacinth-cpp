#pragma once

#include "ast/Program.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
    template <> struct AnalyzerImpl<AST::Program>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::Program &node);
    };

} // namespace Semantic
