#include "semantic/analyzer/impl/Block.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::GlobalBlock>::analyze(Analyzer &analyzer,
                                            AST::GlobalBlock &node)
    {
        if (typeid(node) == typeid(AST::LibBlock))
            return analyzer.analyze(*static_cast<AST::LibBlock *>(&node));

        return {nullptr, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
