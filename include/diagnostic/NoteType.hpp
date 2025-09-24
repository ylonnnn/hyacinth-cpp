#pragma once

#include <cstdint>

namespace Diagnostic
{
    enum class NoteType : uint32_t
    {
        Info = 0xc8,
        Usage,
        Suggestion,
    };

}
