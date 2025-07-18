#pragma once

#include "ast/expr/LiteralExpr.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
    template <> struct AnalyzerImpl<AST::Expr>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::Expr &node);
    };

    template <> struct AnalyzerImpl<AST::LiteralExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::LiteralExpr &node);
    };

} // namespace Semantic
