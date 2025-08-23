#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Stmt>::analyze(Analyzer &analyzer,
                                                    AST::Stmt &node)
    {
        if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(&node))
            return analyzer.analyze(*ptr);

        else if (typeid(node) == typeid(AST::FunctionReturnStmt))
            return analyzer.analyze(
                static_cast<AST::FunctionReturnStmt &>(node));

        else if (typeid(node) == typeid(AST::ExprStmt))
            return analyzer.analyze(static_cast<AST::ExprStmt &>(node));

        else if (typeid(node) == typeid(AST::BlockStmt))
            return analyzer.analyze(static_cast<AST::BlockStmt &>(node));

        return {nullptr, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
