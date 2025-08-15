#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::GlobalStmt>::analyze(Analyzer &analyzer,
                                                          AST::GlobalStmt &node)
    {
        if (typeid(node) == typeid(AST::ImportStmt))
            return AnalyzerImpl<AST::ImportStmt>::analyze(
                analyzer, static_cast<AST::ImportStmt &>(node));

        else if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(&node))
            return AnalyzerImpl<AST::DeclarationStmt>::analyze(analyzer, *ptr);

        return {nullptr, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
