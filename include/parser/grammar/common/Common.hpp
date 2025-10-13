#pragma once

#include "parser/grammar/common/Block.hpp"
#include "parser/grammar/common/Path.hpp"
#include "parser/grammar/common/Terminator.hpp"
#include "parser/grammar/common/pratt/Pratt.hpp"

namespace Parser::Common
{
    static inline Pratt Pratt;
    static inline PathRule PathRule;

    static inline Block Block;
    static inline Terminator Terminator;

} // namespace Parser::Common
