#include "analyzer/Analyzer.hpp"
#include "analyzer/impl/Expr.hpp"
#include "core/type/wrapper/ArrayType.hpp"
#include "core/value/Value.hpp"
#include "core/value/ValuePool.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::ArrayLiteralExpr>::analyze(Analyzer &analyzer,
                                                 AST::ArrayLiteralExpr &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        // Non-inferred array type analysis
        if (node.type != nullptr)
        {
            AnalysisResult t_res =
                AnalyzerImpl<AST::PrefixedType>::analyze(analyzer, *node.type);
            result.adapt(t_res);
        }

        Core::ArrayInstantiated *type =
            result.data != nullptr &&
                    typeid(*result.data) == typeid(Core::ArrayInstantiated)
                ? static_cast<Core::ArrayInstantiated *>(result.data)
                : nullptr;

        Core::InstantiatedType *el_type = nullptr;

        if (type != nullptr)
        {
            Core::GenericArgument &el_ty = type->arguments[0];
            auto ptr = std::get_if<Core::InstantiatedType *>(&el_ty);

            if (ptr == nullptr) [[unlikely]]
                return result;

            el_type = *ptr;
        }

        std::vector<Core::Value *> elements;
        elements.reserve(node.elements.size());

        // Element Analysis
        for (const auto &el : node.elements)
        {
            AnalysisResult e_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, *el);
            result.adapt(e_res.status, std::move(e_res.diagnostics));

            if (type == nullptr)
            {
                el_type = e_res.data;
                type = Core::ArrayType::instance()->create_instance({el_type});

                continue;
            }

            elements.push_back(e_res.value);
            if (e_res.value == nullptr)
                continue;
        }

        result.value =
            Core::create_value(std::make_unique<Core::Value::T>(
                                   Core::array(el_type, std::move(elements))),
                               type, &node.range);

        return result;
    }

} // namespace Semantic
