#pragma once

#include <unordered_map>

#include "core/program/Program.hpp"

namespace Core
{
    class ProgramRegistry
    {
      private:
        Program &main_;
        std::unordered_map<std::string, std::unique_ptr<Program>> programs_;

      public:
        ProgramRegistry(Program &main);

        std::unordered_map<std::string, std::unique_ptr<Program>> &
        programs();

        Program &file(const std::string &path);
    };

} // namespace Core
