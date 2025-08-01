#pragma once

#include <unordered_map>

#include "core/program/Program.hpp"

namespace Core
{
    class ProgramRegistry
    {
      private:
        ProgramFile &main_;
        std::unordered_map<std::string, std::unique_ptr<ProgramFile>> programs_;

      public:
        ProgramRegistry(ProgramFile &main);

        std::unordered_map<std::string, std::unique_ptr<ProgramFile>> &
        programs();

        ProgramFile &file(const std::string &path);
    };

} // namespace Core
