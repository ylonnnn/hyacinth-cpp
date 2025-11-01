#include "analyzer/impl/Common.hpp"
#include "core/symbol/TypeSymbol.hpp"
#include "core/symbol/VariableSymbol.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Identifier>::analyze(Analyzer &analyzer,
                                                          AST::Identifier &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        Core::Environment *current = analyzer.env_stack.current();
        if (current == nullptr) [[unlikely]]
            return result;

        // Analyze identifier arguments
        std::vector<Core::GenericArgument> arguments;
        arguments.reserve(node.arguments.capacity());

        for (auto &arg : node.arguments)
        {
            if (auto ptr = std::get_if<std::unique_ptr<AST::Type>>(&arg))
            {
                AnalysisResult a_res =
                    AnalyzerImpl<AST::Type>::analyze(analyzer, **ptr);
                result.adapt(a_res.status, std::move(a_res.diagnostics));

                arguments.emplace_back(a_res.data);
            }

            else if (auto ptr = std::get_if<std::unique_ptr<AST::Expr>>(&arg))
            {
                AnalysisResult a_res =
                    AnalyzerImpl<AST::Expr>::analyze(analyzer, **ptr);
                result.adapt(a_res.status, std::move(a_res.diagnostics));

                arguments.emplace_back(a_res.value);
            }
        }

        std::string ident(node.identifier.value);
        result.symbol = current->resolve_symbol(ident);

        if (result.symbol == nullptr)
        {
            result.error(Core::PositionRange(node.identifier.range),
                         Diagnostic::ErrorType::UnknownSymbol,
                         "unknown symbol '" + ident + "'.");

            return result;
        }

        Core::Symbol *sym = result.symbol;

        if (typeid(*sym) == typeid(Core::TypeSymbol))
        {
            auto t_sym = static_cast<Core::TypeSymbol *>(sym);
            result.data = t_sym->base.create_instance(std::move(arguments));
        }

        else if (typeid(*sym) == typeid(Core::VariableSymbol))
        {
            auto v_sym = static_cast<Core::VariableSymbol *>(sym);

            result.value = v_sym->value;
            result.data = v_sym->type;
        }

        return result;
    }

} // namespace Semantic
