#include "analyzer/impl/Common.hpp"
#include "core/symbol/TypeSymbol.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Identifier>::analyze(Analyzer &analyzer,
                                                          AST::Identifier &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("impement AST::Identifier analyzer");

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

        result.symbol =
            current->resolve_symbol(std::string(node.identifier.value));
        if (result.symbol == nullptr)
        {
            utils::todo("throw error: unknown symbol '{}'");
            return result;
        }

        if (typeid(*result.symbol) == typeid(Core::TypeSymbol))
        {
            auto t_sym = static_cast<Core::TypeSymbol *>(result.symbol);
            result.data = t_sym->base.create_instance(std::move(arguments));
        }

        return result;
    }

} // namespace Semantic
