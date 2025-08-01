#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/stmt/ExprStmt.hpp"
#include "ast/stmt/GlobalStmt.hpp"
#include "ast/stmt/ImportStmt.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "ast/stmt/function/FunctionRetStmt.hpp"
#include "ast/stmt/types/struct/StructDeclStmt.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"
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

    template <> struct AnalyzerImpl<AST::GlobalStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::GlobalStmt &node);
    };

    template <> struct AnalyzerImpl<AST::ImportStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::ImportStmt &node);
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

    template <> struct AnalyzerImpl<AST::FunctionDeclarationStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::FunctionDeclarationStmt &node);
    };

    template <> struct AnalyzerImpl<AST::FunctionReturnStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::FunctionReturnStmt &node);
    };

    template <> struct AnalyzerImpl<AST::StructDeclarationStmt>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::StructDeclarationStmt &node);
    };

} // namespace Semantic
