#include "ast/type/PrefixedType.hpp"
#include "analyzer/impl/Type.hpp"
#include "core/type/wrapper/ArrayType.hpp"
#include "core/type/wrapper/ReferenceType.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::PrefixedType>::analyze(Analyzer &analyzer,
                                             AST::PrefixedType &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        AST::Type &base = *node.base;

        AnalysisResult b_res = AnalyzerImpl<AST::Type>::analyze(analyzer, base);
        result.adapt(b_res.status, std::move(b_res.diagnostics));

        Core::InstantiatedType *type = b_res.data;
        if (type == nullptr)
        {
            // result.error(Core::PositionRange{  base.position,
            // *base.range.end}, Diagnostic::ErrorType::UnknownSymbol, "");
            return result;
        }

        switch (node.kind)
        {
            case AST::PrefixedTypeKind::Array:
                result.data = Core::ArrayType::instance()->create_instance(
                    {type}, &node.range);

            case AST::PrefixedTypeKind::Reference:
            {
                if (typeid(*type) == typeid(Core::ReferenceInstantiated))
                {
                    // TODO: throw error: cannot have nested references
                    return result;
                }

                result.data = Core::ReferenceType::instance()->create_instance(
                    {type}, &node.range);
            }

            default:
                break;
        }

        utils::todo("impement AST::PrefixedType analyzer");

        return result;
    }

} // namespace Semantic
