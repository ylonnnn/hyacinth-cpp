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
        std::string name(ast_type.value().value);

        Core::BaseType *resolved = current->resolve_type(name);

        if (resolved == nullptr)
        {
            result.error(
                &ast_type, Diagnostic::ErrorTypes::Type::UnrecognizedType,
                std::string("Unrecognized type \"") + Diagnostic::ERR_GEN +
                    name + Utils::Styles::Reset + "\".",
                "Used unrecognized type here");

            return result;
        }

        Core::TypeResolutionResult t_res = resolved->resolve(ast_type);
        result.adapt(t_res.status, std::move(t_res.diagnostics), t_res.data);

        return result;
    }

} // namespace Semantic
