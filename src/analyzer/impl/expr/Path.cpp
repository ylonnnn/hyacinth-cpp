#include <utility>

#include "analyzer/impl/Expr.hpp"
// #include "core/operation/Operation.hpp"
// #include "core/symbol/FunctionSymbol.hpp"
// #include "core/symbol/LibSymbol.hpp"
// #include "core/type/compound/Array.hpp"
// #include "core/type/primitive/Integer.hpp"
#include "lexer/Token.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Path>::analyze(Analyzer &analyzer,
                                                    AST::Path &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("implement AST::Path analyzer");

        return result;
    }

} // namespace Semantic
