#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::ExprStmt>::analyze(Analyzer &analyzer,
                                                        AST::ExprStmt &node)
    {
        return AnalyzerImpl<AST::Expr>::analyze(analyzer, node.expr());
    }

} // namespace Semantic
