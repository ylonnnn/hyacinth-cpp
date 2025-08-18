#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Expr>::analyze(Analyzer &analyzer,
                                                    AST::Expr &node)
    {
        if (typeid(node) == typeid(AST::LiteralExpr))
            return AnalyzerImpl<AST::LiteralExpr>::analyze(
                analyzer, static_cast<AST::LiteralExpr &>(node));

        else if (typeid(node) == typeid(AST::IdentifierExpr))
            return AnalyzerImpl<AST::IdentifierExpr>::analyze(
                analyzer, static_cast<AST::IdentifierExpr &>(node));

        else if (typeid(node) == typeid(AST::BinaryExpr))
            return AnalyzerImpl<AST::BinaryExpr>::analyze(
                analyzer, static_cast<AST::BinaryExpr &>(node));

        else if (typeid(node) == typeid(AST::FunctionCallExpr))
            return AnalyzerImpl<AST::FunctionCallExpr>::analyze(
                analyzer, static_cast<AST::FunctionCallExpr &>(node));

        else if (typeid(node) == typeid(AST::InstanceExpr))
            return AnalyzerImpl<AST::InstanceExpr>::analyze(
                analyzer, static_cast<AST::InstanceExpr &>(node));

        else if (typeid(node) == typeid(AST::ArrayExpr))
            return AnalyzerImpl<AST::ArrayExpr>::analyze(
                analyzer, static_cast<AST::ArrayExpr &>(node));

        return {nullptr, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
