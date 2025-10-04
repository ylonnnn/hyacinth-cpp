#pragma once

#include <cstdint>
#include <type_traits>

namespace Parser
{
    enum GrammarContext : uint8_t
    {
        GC_GLOBAL = (1 << 0),
        GC_LOCAL = (1 << 1),
    };

    inline GrammarContext operator|(GrammarContext a, GrammarContext b)
    {
        using underlying_t = std::underlying_type_t<GrammarContext>;
        return static_cast<GrammarContext>(static_cast<underlying_t>(a) |
                                           static_cast<underlying_t>(b));
    }

    inline GrammarContext operator&(GrammarContext a, GrammarContext b)
    {
        using underlying_t = std::underlying_type_t<GrammarContext>;
        return static_cast<GrammarContext>(static_cast<underlying_t>(a) &
                                           static_cast<underlying_t>(b));
    }

    inline GrammarContext operator^(GrammarContext a, GrammarContext b)
    {
        using underlying_t = std::underlying_type_t<GrammarContext>;
        return static_cast<GrammarContext>(static_cast<underlying_t>(a) ^
                                           static_cast<underlying_t>(b));
    }

    inline GrammarContext operator~(GrammarContext state)
    {
        using underlying_t = std::underlying_type_t<GrammarContext>;
        return static_cast<GrammarContext>(~static_cast<underlying_t>(state));
    }

    inline GrammarContext &operator|=(GrammarContext &a, GrammarContext b)
    {
        a = a | b;
        return a;
    }

    inline GrammarContext &operator&=(GrammarContext &a, GrammarContext b)
    {
        a = a & b;
        return a;
    }

    inline GrammarContext &operator^=(GrammarContext &a, GrammarContext b)
    {
        a = a ^ b;
        return a;
    }

} // namespace Parser
