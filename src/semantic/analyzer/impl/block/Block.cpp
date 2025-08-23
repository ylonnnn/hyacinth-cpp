#include "semantic/analyzer/impl/Block.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Block>::analyze(Analyzer &analyzer,
                                                     AST::Block &node)
    {
        AST::Block *block = &node;

        if (auto ptr = dynamic_cast<AST::GlobalBlock *>(block))
            return analyzer.analyze(*ptr);

        return {nullptr, Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Semantic
