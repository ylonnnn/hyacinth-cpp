#pragma once

#include <bitset>
#include <cstdint>

namespace Core
{
    enum ProgramState : uint8_t
    {
        PFS_MAIN = (1 << 0),
        PFS_VALID = (1 << 1),
        PFS_ANALYZED = (1 << 2),
        PFS_INTERPRETED = (1 << 3),
    };

    class ProgramStateFlags
    {
      private:
        std::bitset<8> bits_;

      public:
        constexpr inline ProgramStateFlags(ProgramState state) { with(state); }

        constexpr inline ProgramStateFlags &with(ProgramState state)
        {
            bits_.set(static_cast<uint8_t>(state));
            return *this;
        }

        constexpr inline bool has(ProgramState state) const
        {
            return bits_.test(static_cast<uint8_t>(state));
        }
    };

} // namespace Core
