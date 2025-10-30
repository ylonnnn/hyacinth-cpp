#pragma once

#include "analyzer/Analyzer.hpp"
#include "ast/type/ModifiedType.hpp"
#include "ast/type/PrefixedType.hpp"
#include "ast/type/SimpleType.hpp"
#include "ast/type/SuffixedType.hpp"
#include "ast/type/Type.hpp"

namespace Semantic
{
    template <> struct AnalyzerImpl<AST::Type>
    {
        static AnalysisResult analyze(Analyzer &analyzer, AST::Type &node);
    };

    template <> struct AnalyzerImpl<AST::ModifiedType>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::ModifiedType &node);
    };

    template <> struct AnalyzerImpl<AST::PrefixedType>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::PrefixedType &node);
    };

    template <> struct AnalyzerImpl<AST::SuffixedType>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::SuffixedType &node);
    };

    template <> struct AnalyzerImpl<AST::SimpleType>
    {
        static AnalysisResult analyze(Analyzer &analyzer,
                                      AST::SimpleType &node);
    };

} // namespace Semantic
