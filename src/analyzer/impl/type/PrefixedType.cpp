#include "ast/type/PrefixedType.hpp"
#include "analyzer/impl/Type.hpp"
#include "core/type/wrapper/ArrayType.hpp"
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
        result.adapt(b_res);

        Core::InstantiatedType *type = result.data;
        if (type == nullptr)
        {
            // result.error(Core::PositionRange{  base.position,
            // *base.range.end}, Diagnostic::ErrorType::UnknownSymbol, "");
            return result;
        }

        switch (node.kind)
        {
            case AST::PrefixedTypeKind::Array:
            {
                // Core::ArrayType::instance()->create_instance();
            }

            default:
                break;
        }

        utils::todo("impement AST::PrefixedType analyzer");

        return result;
    }

} // namespace Semantic
