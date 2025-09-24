#include "ast/type/SimpleType.hpp"
#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::IdentifierExpr>::analyze(Analyzer &analyzer,
                                               AST::IdentifierExpr &node)
    {
        Core::Environment *current = analyzer.current_env();
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        std::string identifier(node.identifier().value);
        Core::Symbol *symbol = current->resolve_symbol(identifier);

        if (symbol != nullptr)
        {
            result.symbol = symbol;

            if (auto ptr = dynamic_cast<Core::IdentifierSymbol *>(symbol))
            {
                result.value = ptr->value;
                result.data = ptr->type;

                node.set_value(result.value);
            }

            return result;
        }

        else
        {
            std::cout << "no sym\n";
            auto t_node = AST::TypeExpr(
                std::make_unique<AST::SimpleType>(node.identifier()));
            AnalysisResult ta_res =
                AnalyzerImpl<AST::TypeExpr>::analyze(analyzer, t_node);
            result.adapt(ta_res.status, {}, ta_res.data);

            result.value = ta_res.value;
            result.symbol = ta_res.symbol;

            if (result.status == Core::ResultStatus::Fail)
                result.error(
                    &node, Diagnostic::ErrorTypes::Semantic::UnrecognizedSymbol,
                    std::string("Unrecognized symbol \"") +
                        Diagnostic::ERR_GEN + identifier +
                        utils::Styles::Reset + "\" provided.",
                    "Used unrecognized symbol here");

            return result;
        }
    }

} // namespace Semantic
