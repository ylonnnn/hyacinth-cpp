#include <typeindex>

#include "analyzer/Analyzer.hpp"
#include "analyzer/impl/Type.hpp"

#define TYPE_DISPATCH(T) DISPATCH(T, AST::Type)

namespace Semantic
{
    static const std::unordered_map<std::type_index, AnalyzerImplFn<AST::Type>>
        type_dispatch_table = {

            // ModifiedType
            TYPE_DISPATCH(AST::ModifiedType),

            // PrefixedType
            TYPE_DISPATCH(AST::PrefixedType),

            // SuffixedType
            TYPE_DISPATCH(AST::SuffixedType),

            // SimpleType
            TYPE_DISPATCH(AST::SimpleType),
    };

    AnalysisResult AnalyzerImpl<AST::Type>::analyze(Analyzer &analyzer,
                                                    AST::Type &node)
    {
        auto it = type_dispatch_table.find(typeid(node));
        return it == type_dispatch_table.end()
                   ? AnalysisResult{nullptr,
                                    Core::ResultStatus::Fail,
                                    nullptr,
                                    {}}
                   : it->second(analyzer, node);
    }

} // namespace Semantic
