#pragma once

#include <cstdint>

namespace Diagnostic
{
    enum class WarningType : uint32_t
    {
        Unused = 0x12c,
        Unreachable,
    };

}
