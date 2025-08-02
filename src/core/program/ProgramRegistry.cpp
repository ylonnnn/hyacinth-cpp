#include "core/program/ProgramRegistry.hpp"
#include "core/program/Program.hpp"

namespace Core
{
    ProgramRegistry::ProgramRegistry(ProgramFile &main) : main_(main)
    {
        main_.registry_ = this;
    }

    std::unordered_map<std::string, std::unique_ptr<ProgramFile>> &
    ProgramRegistry::programs()
    {
        return programs_;
    }

    ProgramFile &ProgramRegistry::file(const std::string &path)
    {

        auto program = std::make_unique<ProgramFile>(path.c_str(), false);
        program->registry_ = this;

        auto [it, _] = programs_.emplace(path, std::move(program));

        return *it->second;
    }

} // namespace Core
