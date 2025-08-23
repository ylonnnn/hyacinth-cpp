#include "ast/stmt/BlockStmt.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::BlockStmt>::analyze(Analyzer &analyzer,
                                                         AST::BlockStmt &node)
    {
        return analyzer.analyze(node.block());
    }

} // namespace Semantic
