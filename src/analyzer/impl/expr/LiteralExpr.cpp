#include <iostream>

#include "analyzer/Analyzer.hpp"
#include "analyzer/impl/Expr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "utils/dev.hpp"
#include "utils/value.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::LiteralExpr>::analyze(Analyzer &analyzer,
                                            AST::LiteralExpr &node)
    {
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        result.value = utils::parse_val(node.value);
        if (result.value != nullptr)
            utils::todo("implement type inference from value");

        return result;
    }

} // namespace Semantic
