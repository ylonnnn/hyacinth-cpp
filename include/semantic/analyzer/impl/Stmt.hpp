#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/stmt/ExprStmt.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "ast/stmt/variable/VariableDefStmt.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
    template <> struct AnalyzerImpl<AST::Stmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::Stmt &node);
    };

    template <> struct AnalyzerImpl<AST::ExprStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::ExprStmt &node);
    };

    template <> struct AnalyzerImpl<AST::DeclarationStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::DeclarationStmt &node);
    };

    template <> struct AnalyzerImpl<AST::VariableDeclarationStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::VariableDeclarationStmt &node);
    };

    template <> struct AnalyzerImpl<AST::VariableDefinitionStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::VariableDefinitionStmt &node);
    };

    template <> struct AnalyzerImpl<AST::FunctionDeclarationStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::FunctionDeclarationStmt &node);
    };

} // namespace Semantic
