#include "ast/stmt/BlockStmt.hpp"
#include "analyzer/impl/Stmt.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::BlockStmt>::analyze(Analyzer &analyzer,
                                                         AST::BlockStmt &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("implement AST::BlockStmt analyzer");

        return result;
    }

} // namespace Semantic
