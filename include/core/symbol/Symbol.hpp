#pragma once

#include <optional>
#include <string>

#include "core/program/Program.hpp"

namespace Core
{
    struct Symbol
    {
        std::string name;
        Core::Position declared_at;
        std::optional<Core::Position> defined_at = std::nullopt;
    };

} // namespace Core
