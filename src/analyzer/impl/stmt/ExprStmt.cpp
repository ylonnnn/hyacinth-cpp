#include "analyzer/impl/Stmt.hpp"
#include "ast/expr/FunctionCallExpr.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::ExprStmt>::analyze(Analyzer &analyzer,
                                                        AST::ExprStmt &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("implement AST::ExprStmt analyzer");

        return result;

        // AST::Expr &expr = node.expr();
        // AnalysisResult result = analyzer.analyze(expr);

        // if (typeid(expr) != typeid(AST::FunctionCallExpr))
        //     result.warn(&expr, Diagnostic::WarningType::Unused,
        //                 "Expression result unused", "Expression unused");

        // return result;
    }

} // namespace Semantic
