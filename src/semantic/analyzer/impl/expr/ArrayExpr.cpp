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

        for (auto &element : node.elements())
        {
            AnalysisResult a_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, *element);
            result.adapt(a_res.status, std::move(a_res.diagnostics));

            std::cout << "a_res.value: "
                      << (a_res.value ? *a_res.value : "nullptr") << "\n";
            std::cout << "a_res.value[]: "
                      << (a_res.value
                              ? Core::Type::from_value(current, *a_res.value)
                              : nullptr)
                      << "\n";
            std::cout << "a_res.data: "
                      << (a_res.data ? a_res.data->to_string() : "nullptr")
                      << "\n";

            el_type =
                el_type == nullptr
                    ? a_res.data
                    : (el_type->assignable_with(*a_res.data)
                           ? el_type
                           : (a_res.data->assignable_with(*el_type) ? a_res.data
                                                                    : el_type));

            if ((a_res.value != nullptr && el_type->assignable(*a_res.value)) ||
                (a_res.data != nullptr &&
                 el_type->assignable_with(*a_res.data)))
            {
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

        return result;
    }

} // namespace Semantic
