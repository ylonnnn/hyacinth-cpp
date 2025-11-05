#pragma once

#include <iostream>
#include <variant>

#include "core/program/Program.hpp"

namespace Core
{
    struct Position
    {
        size_t row, col, offset;

        /**
         * Reference wrappers to allow movability and non-nullability
         */
        std::reference_wrapper<Program> program;
    };

    struct PositionRange
    {
        using Pos = std::variant<Position, Position *>;

        PositionRange(Pos start, Pos end);

        Position &start();
        const Position &start() const;

        void start(Position &start);
        void start(Position &&start);

        Position &end();
        const Position &end() const;

        void end(Position &end);
        void end(Position &&end);

      private:
        /**
         * Variant of Position and Position * to support custom created
         * positions or hold existing ones
         */
        Pos start_, end_;
    };

    /**
     * NOTE: `start` and `end` are exclusive unless the difference between the
     * two positions is less than 1
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

        return PositionRange(start_, end_);
    }

    static inline std::ostream &operator<<(std::ostream &os,
                                           const Position &position)
    {
        return os << "[" << position.row << ":" << position.col << ":"
                  << position.offset << "]";
    }

} // namespace Core
