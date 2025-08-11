#include <iostream>

#include "ast/expr/LiteralExpr.hpp"
#include "semantic/analyzer/Analyzer.hpp"
#include "semantic/analyzer/impl/Expr.hpp"
#include "utils/value.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::LiteralExpr>::analyze(Analyzer &analyzer,
                                            AST::LiteralExpr &node)
    {
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        result.value = Utils::parse_val(node.value());
        if (result.value)
            result.data =
                Core::Type::from_value(&analyzer.environment(), *result.value);

        return result;
    }

} // namespace Semantic
