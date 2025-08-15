#include "semantic/analyzer/Analyzer.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::DeclarationStmt>::analyze(Analyzer &analyzer,
                                                AST::DeclarationStmt &node)
    {
        AST::DeclarationStmt *stmt = &node;

        if (auto ptr = dynamic_cast<AST::VariableDeclarationStmt *>(stmt))
            return AnalyzerImpl<AST::VariableDeclarationStmt>::analyze(analyzer,
                                                                       *ptr);

        else if (auto ptr = dynamic_cast<AST::FunctionDeclarationStmt *>(stmt))
            return AnalyzerImpl<AST::FunctionDeclarationStmt>::analyze(analyzer,
                                                                       *ptr);

        else if (auto ptr = dynamic_cast<AST::StructDeclarationStmt *>(stmt))
            return AnalyzerImpl<AST::StructDeclarationStmt>::analyze(analyzer,
                                                                     *ptr);

        return {nullptr, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
