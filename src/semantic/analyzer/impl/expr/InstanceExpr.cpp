#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::InstanceExpr>::analyze(Analyzer &analyzer,
                                             AST::InstanceExpr &node)
    {
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        Core::object obj;

        for (const auto &[name, field] : node.fields())
        {
            AnalysisResult v_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, *field);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            obj.set(name,
                    Core::object_entry{std::move(v_res.value), v_res.data});
        }

        result.value = std::move(obj);

        return result;
    }

} // namespace Semantic
