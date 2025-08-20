#include "core/value/Value.hpp"
#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::ArrayExpr>::analyze(Analyzer &analyzer,
                                                         AST::ArrayExpr &node)
    {
        AnalysisResult result = {
            std::make_shared<Core::Value>(Core::array{nullptr}),
            Core::ResultStatus::Success,
            nullptr,
            {}};

        Core::Environment *current = analyzer.current_env();
        auto arr = std::get_if<Core::array>(result.value.get());

        Core::Type *&el_type = arr->element_type();
        auto bind = true;

        for (auto &element : node.elements())
        {
            AnalysisResult a_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, *element);
            result.adapt(a_res.status, std::move(a_res.diagnostics));

            el_type =
                el_type == nullptr
                    ? a_res.data
                    : (el_type->assignable_with(*a_res.data)
                           ? el_type
                           : (a_res.data->assignable_with(*el_type) ? a_res.data
                                                                    : el_type));
            if (el_type == nullptr)
                break;

            if ((a_res.value != nullptr && el_type->assignable(*a_res.value)) ||
                (a_res.data != nullptr &&
                 el_type->assignable_with(*a_res.data)))
            {
                if (a_res.value == nullptr)
                    bind = false;

                arr->elements().emplace_back(
                    Core::value_data{a_res.value, a_res.data});

                continue;
            }

            result.error(element.get(), Diagnostic::ErrorTypes::Type::Mismatch,
                         std::string("Expected value assignable to type \"") +
                             Diagnostic::ERR_GEN + el_type->to_string() +
                             Utils::Styles::Reset + "\".",
                         "");
        }

        result.data = Core::Type::from_value(current, *result.value);

        if (bind)
            node.set_value(result.value);

        return result;
    }

} // namespace Semantic
