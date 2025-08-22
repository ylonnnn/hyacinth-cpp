#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Stmt>::analyze(Analyzer &analyzer,
                                                    AST::Stmt &node)
    {
        AST::Stmt *stmt = &node;

        if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(stmt))
            return analyzer.analyze(*ptr);

        else if (typeid(*stmt) == typeid(AST::FunctionReturnStmt))
            return AnalyzerImpl<AST::FunctionReturnStmt>::analyze(
                analyzer, *static_cast<AST::FunctionReturnStmt *>(stmt));

        else if (typeid(*stmt) == typeid(AST::ExprStmt))
            return AnalyzerImpl<AST::ExprStmt>::analyze(
                analyzer, *static_cast<AST::ExprStmt *>(stmt));

        return {nullptr, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
