#include "core/position/Position.hpp"

namespace Core
{
    PositionRange::PositionRange(Pos start, Pos end)
        : start_(std::move(start)), end_(std::move(end))
    {
    }

    Position &PositionRange::start()
    {
        return std::visit(
            [](auto &val) -> Position &
            {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, Position *>)
                    return *val;
                else
                    return val;
            },
            start_);
    }

    const Position &PositionRange::start() const
    {
        return const_cast<PositionRange *>(this)->start();
    }

    void PositionRange::start(Position &start) { start_ = &start; }

    void PositionRange::start(Position &&start) { start_ = std::move(start); }

    Position &PositionRange::end()
    {
        return std::visit(
            [](auto &&val) -> Position &
            {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, Position *>)
                    return *val;
                else
                    return val;
            },
            end_);
    }

    const Position &PositionRange::end() const
    {
        return const_cast<PositionRange *>(this)->end();
    }

    void PositionRange::end(Position &end) { end_ = &end; }

    void PositionRange::end(Position &&end) { end_ = std::move(end); }

} // namespace Core
