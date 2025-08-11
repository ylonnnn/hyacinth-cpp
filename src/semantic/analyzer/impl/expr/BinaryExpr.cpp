#include "core/operation/Operation.hpp"
#include "semantic/analyzer/impl/Expr.hpp"
#include <utility>

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::BinaryExpr>::analyze(Analyzer &analyzer,
                                                          AST::BinaryExpr &node)
    {
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        auto op = node.operation().type;

        AnalysisResult l_res =
            AnalyzerImpl<AST::Expr>::analyze(analyzer, node.left());

        result.adapt(l_res.status, std::move(l_res.diagnostics));
        if (result.status == Core::ResultStatus::Fail)
        {

            return result;
        }

        AnalysisResult r_res =
            AnalyzerImpl<AST::Expr>::analyze(analyzer, node.right());

        result.adapt(r_res.status, std::move(r_res.diagnostics));
        if (result.status == Core::ResultStatus::Fail)
        {

            return result;
        }

        auto [operation, flipped] =
            Core::Operation::get_binary_overload({op, l_res.data, r_res.data});

        if (operation == nullptr)
        {
            result.error(
                &node,
                Diagnostic::ErrorTypes::Semantic::NoViableOperatorOverload,
                std::string("No viable operator overload for ") +
                    Diagnostic::ERR_GEN + l_res.data->to_string() + " " +
                    std::string(node.operation().value) + " " +
                    r_res.data->to_string() + Utils::Styles::Reset + ".",
                "");

            return result;
        }

        if (flipped)
            std::swap(l_res, r_res);

        std::optional<Core::Operation::Assignee> res_val = operation->handler(
            Core::Operation::Operand{l_res.data, l_res.value},
            Core::Operation::Operand{r_res.data, r_res.value});

        if (!res_val)
        {
            // result.error();
            return result;
        }

        result.data = res_val->type;
        result.value = res_val->value;

        std::cout << "type: "
                  << (result.data != nullptr ? result.data->to_string()
                                             : "none")
                  << "\n";

        std::cout << "value: " << (result.value ? *result.value : "none")
                  << "\n";

        return result;
    }

} // namespace Semantic
