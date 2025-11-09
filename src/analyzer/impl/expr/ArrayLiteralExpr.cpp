#include "analyzer/Analyzer.hpp"
#include "analyzer/impl/Expr.hpp"
#include "core/type/wrapper/ArrayType.hpp"

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

            Core::TypeResult ev_res = el_type->assignable(e_res.value);
            result.adapt(ev_res.status, std::move(ev_res.diagnostics));
        }

        return result;
    }

} // namespace Semantic
