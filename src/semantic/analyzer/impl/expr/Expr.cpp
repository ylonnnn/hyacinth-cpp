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

        else if (typeid(node) == typeid(AST::FunctionCalLExpr))
            return AnalyzerImpl<AST::FunctionCalLExpr>::analyze(
                analyzer, static_cast<AST::FunctionCalLExpr &>(node));

        else if (typeid(node) == typeid(AST::InstanceExpr))
            return AnalyzerImpl<AST::InstanceExpr>::analyze(
                analyzer, static_cast<AST::InstanceExpr &>(node));

        return {std::nullopt, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
