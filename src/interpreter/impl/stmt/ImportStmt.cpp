#include "core/program/ProgramRegistry.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Stmt.hpp"

namespace Interpreter
{
    namespace fs = std::filesystem;

    InterpretationResult
    InterpreterImpl<AST::ImportStmt>::interpret(Interpreter &interpreter,
                                                AST::ImportStmt &node)
    {
        InterpretationResult result = {
            Core::ResultStatus::Success, nullptr, {}};

        std::string target(node.target_strv());
        fs::path path = target.substr(1, target.size() - 2);

        Core::ProgramFile &current = interpreter.program();
        auto &programs = current.registry()->programs();

        auto target_program =
            (path.is_relative() ? current.path().parent_path() / path : path)
                .string();
        auto it = programs.find(target_program);

        [[unlikely]]
        if (it == programs.end())
        {
            result.error(
                &node, Diagnostic::ErrorTypes::Runtime::UnresolvedImport,
                std::string("Unresolved import for \"") + Diagnostic::ERR_GEN +
                    target_program + Utils::Styles::Reset + "\".",
                "");

            return result;
        }

        InterpretationResult i_res = it->second->interpret(it->second->node());
        result.adapt(i_res.status, std::move(i_res.diagnostics));

        return result;
    }

} // namespace Interpreter
