#include "core/environment/Environment.hpp"
#include "core/environment/LibEnvironment.hpp"
#include "core/program/DependencyEnvironment.hpp"
#include "core/symbol/LibSymbol.hpp"
#include "semantic/analyzer/impl/Block.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::LibBlock>::analyze(Analyzer &analyzer,
                                                        AST::LibBlock &node)
    {
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        Core::Environment *current = analyzer.current_env();
        Core::LibSymbol *lib = nullptr;

        std::string l_name(node.name().value);

        Core::Symbol *resolved = current->resolve_symbol(l_name);
        if (resolved == nullptr)
        {
            auto sym = std::make_unique<Core::LibSymbol>(
                node.name().value, Core::SymbolAccessibility::Public,
                node.position(), &node);

            lib = sym.get();
            analyzer.program().dependencies().declare_symbol(std::move(sym));

            auto lib_env = std::make_unique<Core::LibEnvironment>(current, lib);
            lib->environment = lib_env.get();

            current->children().emplace(l_name, std::move(lib_env));
        }
        else
        {
            if (typeid(*resolved) == typeid(Core::LibSymbol))
                lib = static_cast<Core::LibSymbol *>(resolved);
        }

        if (lib == nullptr)
        {
            // TODO: Throw unknown library error
            return result;
        }

        analyzer.set_current_env(*lib->environment);

        for (const auto &statement : node.statements())
        {
            AnalysisResult sa_res = analyzer.analyze(*statement);
            result.adapt(sa_res.status, std::move(sa_res.diagnostics));
        }

        analyzer.set_current_env(*current);

        return result;
    }

} // namespace Semantic
