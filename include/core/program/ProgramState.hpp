#pragma once

#include <cstdint>
#include <type_traits>

namespace Core
{
    enum ProgramState : uint32_t
    {
        PFS_MAIN = (1 << 0),
        PFS_VALID = (1 << 1),
        PFS_ANALYZED = (1 << 2),
        PFS_INTERPRETED = (1 << 3),
    };

    inline ProgramState operator|(ProgramState a, ProgramState b)
    {
        using underlying_t = std::underlying_type_t<ProgramState>;
        return static_cast<ProgramState>(static_cast<underlying_t>(a) |
                                         static_cast<underlying_t>(b));
    }

    inline ProgramState operator&(ProgramState a, ProgramState b)
    {
        using underlying_t = std::underlying_type_t<ProgramState>;
        return static_cast<ProgramState>(static_cast<underlying_t>(a) &
                                         static_cast<underlying_t>(b));
    }

    inline ProgramState operator^(ProgramState a, ProgramState b)
    {
        using underlying_t = std::underlying_type_t<ProgramState>;
        return static_cast<ProgramState>(static_cast<underlying_t>(a) ^
                                         static_cast<underlying_t>(b));
    }

    inline ProgramState operator~(ProgramState state)
    {
        using underlying_t = std::underlying_type_t<ProgramState>;
        return static_cast<ProgramState>(~static_cast<underlying_t>(state));
    }

    inline ProgramState &operator|=(ProgramState &a, ProgramState b)
    {
        a = a | b;
        return a;
    }

    inline ProgramState &operator&=(ProgramState &a, ProgramState b)
    {
        a = a & b;
        return a;
    }

    inline ProgramState &operator^=(ProgramState &a, ProgramState b)
    {
        a = a ^ b;
        return a;
    }

} // namespace Core
