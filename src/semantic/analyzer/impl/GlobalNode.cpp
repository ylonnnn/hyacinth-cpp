#include "ast/block/GlobalBlock.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::GlobalNode>::analyze(Analyzer &analyzer,
                                                          AST::GlobalNode &node)
    {
        if (typeid(node) == typeid(AST::ImportStmt))
            return AnalyzerImpl<AST::ImportStmt>::analyze(
                analyzer, static_cast<AST::ImportStmt &>(node));

        else if (auto ptr = dynamic_cast<AST::GlobalBlock *>(&node))
            return analyzer.analyze(*ptr);

        else if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(&node))
            return analyzer.analyze(*ptr);

        return {nullptr, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
