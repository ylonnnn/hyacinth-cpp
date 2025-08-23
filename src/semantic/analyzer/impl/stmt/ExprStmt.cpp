#include "ast/expr/FunctionCallExpr.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::ExprStmt>::analyze(Analyzer &analyzer,
                                                        AST::ExprStmt &node)
    {
        AST::Expr &expr = node.expr();
        AnalysisResult result = analyzer.analyze(expr);

        if (typeid(expr) != typeid(AST::FunctionCallExpr))
            result.warn(&expr, Diagnostic::WarningType::Unused,
                        "Expression result unused", "Expression unused");

        return result;
    }

} // namespace Semantic
