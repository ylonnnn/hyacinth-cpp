#pragma once

#include "core/program/Program.hpp"

namespace Core
{
    struct Position
    {
        size_t row, col, offset;
        ProgramFile &program;
    };

    struct PositionRefRange
    {
        Position &start, &end;
    };

    struct PositionRange
    {
        Position start, end;
    };

} // namespace Core
