#include "ast/type/ScopedType.hpp"
#include "core/symbol/LibSymbol.hpp"
#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::TypeExpr>::analyze(Analyzer &analyzer,
                                                        AST::TypeExpr &node)
    {
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};
        Core::Environment *current = analyzer.current_env();

        // TODO: TypeExpr Analysis (Generally for Static Member Access)

        AST::Type &ast_type = node.type();

        Core::TypeResolutionResult t_res = current->resolve_ast_type(ast_type);
        result.adapt(t_res.status, std::move(t_res.diagnostics), t_res.data);

        analyzer.set_current_env(*current);

        return result;
    }

} // namespace Semantic
