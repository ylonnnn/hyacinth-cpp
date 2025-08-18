#pragma once

#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/FunctionCallExpr.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/expr/TypeExpr.hpp"
#include "ast/expr/UnaryExpr.hpp"
#include "ast/expr/compound/ArrayExpr.hpp"
#include "ast/expr/compound/InstanceExpr.hpp"
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

    template <> struct AnalyzerImpl<AST::UnaryExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::UnaryExpr &node);
    };

    template <> struct AnalyzerImpl<AST::BinaryExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::BinaryExpr &node);
    };

    template <> struct AnalyzerImpl<AST::FunctionCallExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::FunctionCallExpr &node);
    };

    template <> struct AnalyzerImpl<AST::InstanceExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::InstanceExpr &node);
    };

    template <> struct AnalyzerImpl<AST::ArrayExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::ArrayExpr &node);
    };

    template <> struct AnalyzerImpl<AST::TypeExpr>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::TypeExpr &node);
    };

} // namespace Semantic
