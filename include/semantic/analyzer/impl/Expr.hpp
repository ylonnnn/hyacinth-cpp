#pragma once

#include "ast/expr/FunctionCallExpr.hpp"
#include "ast/expr/IdentifierExpr.hpp"
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

    template <> struct AnalyzerImpl<AST::IdentifierExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::IdentifierExpr &node);
    };

    template <> struct AnalyzerImpl<AST::FunctionCalLExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::FunctionCalLExpr &node);
    };

} // namespace Semantic
