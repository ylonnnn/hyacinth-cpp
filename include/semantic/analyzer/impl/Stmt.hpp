#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/stmt/VariableDeclStmt.hpp"
#include "ast/stmt/VariableDefStmt.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
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

} // namespace Semantic
