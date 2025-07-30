#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::IdentifierExpr>::analyze(Analyzer &analyzer,
                                               AST::IdentifierExpr &node)
    {
        Core::Environment *current = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Fail, nullptr, {}};

        // TODO: Allow General Symbol Values
        // Core::Symbol *symbol =
        //     current.resolve_symbol(std::string(node.identifier().value));

        std::string identifier(node.identifier().value);
        Core::Symbol *symbol = current->resolve_symbol(identifier);

        if (symbol != nullptr)
        {
            result.symbol = symbol;
            result.status = Core::ResultStatus::Success;

            if (auto ptr = dynamic_cast<Core::IdentifierSymbol *>(symbol))
            {
                result.value = ptr->value;
                result.data = ptr->type_();
            }

            return result;
        }

        else
        {
            result.error(
                &node, Diagnostic::ErrorTypes::Semantic::UnrecognizedSymbol,
                std::string("Unrecognized symbol \"") + Diagnostic::ERR_GEN +
                    identifier + Utils::Styles::Reset + "\" provided.",
                "Used unrecognized symbol here");

            return result;
        }
    }

} // namespace Semantic
