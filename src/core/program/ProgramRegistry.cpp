#include "core/program/ProgramRegistry.hpp"
#include "core/program/Program.hpp"
#include "core/program/ProgramState.hpp"

namespace Core
{
    ProgramRegistry::ProgramRegistry(Program &main) : main_(main)
    {
        main_.registry_ = this;
    }

    std::unordered_map<std::string, std::unique_ptr<Program>> &
    ProgramRegistry::programs()
    {
        return programs_;
    }

    Program &ProgramRegistry::file(const std::string &path)
    {
        auto program =
            std::make_unique<Program>(path, static_cast<ProgramState>(0));
        program->registry_ = this;

        auto [it, _] = programs_.emplace(path, std::move(program));

        return *it->second;
    }

} // namespace Core
