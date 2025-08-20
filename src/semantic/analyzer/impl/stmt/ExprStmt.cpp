#include "ast/expr/FunctionCallExpr.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::ExprStmt>::analyze(Analyzer &analyzer,
                                                        AST::ExprStmt &node)
    {
        AST::Expr &expr = node.expr();
        AnalysisResult result = analyzer.analyze(expr);

        std::cout << "result status: " << result.status << "\n";

        if (typeid(expr) != typeid(AST::FunctionCallExpr))
            result.warn(&expr, Diagnostic::WarningType::Unused,
                        "Expression result unused", "Expression unused");

        std::cout << "expr stmt: " << result.status << "\n";

        return result;
    }

} // namespace Semantic
