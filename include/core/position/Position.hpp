#pragma once

#include "core/program/Program.hpp"
#include <iostream>

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

    /**
     * NOTE: `start` and `end` are exclusive
     */
    static inline PositionRange range_between(const Position &start,
                                              const Position &end)
    {
        Position start_ = start, end_ = end;

        // If the range will not overlap and become too small, exclude the
        // starting and ending characters
        if (static_cast<int32_t>(end_.col) - static_cast<int32_t>(start_.col) >
            1)
        {
            ++start_.col;
            --end_.col;
        }

        return PositionRange{
            .start = start_,
            .end = end_,
        };
    }

    static inline std::ostream &operator<<(std::ostream &os,
                                           const Position &position)
    {
        return os << "[" << position.row << ":" << position.col << ":"
                  << position.offset << "]";
    }

} // namespace Core
