#pragma once

#include "core/program/Program.hpp"

namespace Core
{
    struct Position
    {
        size_t row, col, offset;
        Program &program;
    };

    struct PositionRefRange
    {
        Position &start, &end;
    };

    struct PositionRange
    {
        Position start, end;
    };

    static inline std::ostream &operator<<(std::ostream &os,
                                           const Position &position)
    {
        return os << "[" << position.row << ":" << position.col << ":"
                  << position.offset << "]";
    }

} // namespace Core
