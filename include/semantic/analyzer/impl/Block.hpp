#pragma once

#include "ast/block/Block.hpp"
#include "ast/block/GlobalBlock.hpp"
#include "ast/block/LibBlock.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
    template <> struct AnalyzerImpl<AST::Block>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::Block &node);
    };

    template <> struct AnalyzerImpl<AST::GlobalBlock>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::GlobalBlock &node);
    };

    template <> struct AnalyzerImpl<AST::LibBlock>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::LibBlock &node);
    };

} // namespace Semantic
