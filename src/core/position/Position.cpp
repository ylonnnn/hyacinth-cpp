#include "core/position/Position.hpp"

namespace Core
{
    PositionRange::PositionRange(Pos start, Pos end) : start_(start), end_(end)
    {
    }

    Position &PositionRange::start()
    {
        if (auto ptr = std::get_if<Position>(&start_))
            return *ptr;
        else
            return **std::get_if<Position *>(&start_);
    }

    const Position &PositionRange::start() const
    {
        return const_cast<PositionRange *>(this)->start();
    }

    void PositionRange::start(Position &start) { start_ = &start; }

    void PositionRange::start(Position &&start) { start_ = std::move(start); }

    Position &PositionRange::end()
    {
        if (auto ptr = std::get_if<Position>(&end_))
            return *ptr;
        else
            return **std::get_if<Position *>(&end_);
    }

    const Position &PositionRange::end() const
    {
        return const_cast<PositionRange *>(this)->end();
    }

} // namespace Core
