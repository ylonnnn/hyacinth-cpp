#include "core/program/DependencyEnvironment.hpp"
#include "core/symbol/LibSymbol.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Block.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::LibBlock>::interpret(Interpreter &interpreter,
                                              AST::LibBlock &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, nullptr, {}};

        Core::Environment *current = interpreter.current_env();
        Core::LibSymbol *lib = nullptr;

        std::string l_name(node.name().value);

        Core::Symbol *resolved = current->resolve_symbol(l_name);
        if (resolved == nullptr)
        {
            auto sym = std::make_unique<Core::LibSymbol>(
                node.name().value, Core::SymbolAccessibility::Public,
                node.position(), &node);

            lib = sym.get();
            interpreter.program().dependencies().declare_symbol(std::move(sym));

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

        interpreter.set_current_env(*lib->environment);

        for (const auto &statement : node.statements())
        {
            InterpretationResult si_res = interpreter.interpret(*statement);
            result.adapt(si_res.status, std::move(si_res.diagnostics));
        }

        interpreter.set_current_env(*current);

        return result;
    }

} // namespace Interpreter
