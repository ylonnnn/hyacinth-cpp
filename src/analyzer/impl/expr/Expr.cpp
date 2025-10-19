#include <typeindex>

#include "analyzer/Analyzer.hpp"
#include "analyzer/impl/Expr.hpp"

#define EXPR_DISPATCH(T) DISPATCH(T, AST::Expr)

namespace Semantic
{
    static const std::unordered_map<std::type_index, AnalyzerImplFn<AST::Expr>>
        expr_dispatch_table = {

            // LiteralExpr
            EXPR_DISPATCH(AST::LiteralExpr),

            // Path
            EXPR_DISPATCH(AST::Path),

            // UnaryExpr
            EXPR_DISPATCH(AST::UnaryExpr),

            // BinaryExpr
            EXPR_DISPATCH(AST::BinaryExpr),

            // FunctionCallExpr
            EXPR_DISPATCH(AST::FunctionCallExpr),
    };

    AnalysisResult AnalyzerImpl<AST::Expr>::analyze(Analyzer &analyzer,
                                                    AST::Expr &node)
    {
        auto it = expr_dispatch_table.find(typeid(node));
        return it == expr_dispatch_table.end()
                   ? AnalysisResult{nullptr,
                                    Core::ResultStatus::Fail,
                                    nullptr,
                                    {}}
                   : it->second(analyzer, node);
    }

} // namespace Semantic
