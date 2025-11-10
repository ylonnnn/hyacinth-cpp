#include <utility>

#include "analyzer/impl/Expr.hpp"
// #include "core/operation/Operation.hpp"
// #include "core/symbol/FunctionSymbol.hpp"
// #include "core/symbol/LibSymbol.hpp"
// #include "core/type/compound/Array.hpp"
// #include "core/type/primitive/Integer.hpp"
#include "core/symbol/PetalSymbol.hpp"
#include "core/symbol/TypeSymbol.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Path>::analyze(Analyzer &analyzer,
                                                    AST::Path &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        Core::EnvironmentStack &env_stack = analyzer.env_stack;
        Core::Environment *top = env_stack.current();

        for (auto &segment : node.segments)
        {
            AnalysisResult s_res =
                AnalyzerImpl<AST::Identifier>::analyze(analyzer, *segment);
            result.adapt(s_res);

            Core::Symbol *sym = result.symbol;
            if (sym == nullptr)
                break;

            if (typeid(*sym) == typeid(Core::PetalSymbol))
            {
                auto p_sym = static_cast<Core::PetalSymbol *>(sym);
                env_stack.enter(*p_sym->environment);
            }

            else if (typeid(*sym) == typeid(Core::TypeSymbol))
            {
                // TODO: Implement Core::TypeSymbol path resolver. For static
                // member access (e.g. std::vector<int<{8}>>::iterator)

                // utils::todo(
                //     "implement Core::TypeSymbol path resolver. For static "
                //     "member access (e.g. std::vector<int<{8}>>::iterator)");
                break;
            }
        }

        while (env_stack.current() != top)
            env_stack.exit();

        return result;
    }

} // namespace Semantic
