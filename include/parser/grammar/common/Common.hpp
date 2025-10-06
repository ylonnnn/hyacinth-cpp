#pragma once

// #include "parser/grammar/common/Block.hpp"
// #include "parser/grammar/common/IdentifierInit.hpp"
// #include "parser/grammar/common/Mutability.hpp"
#include "parser/grammar/common/Terminator.hpp"
#include "parser/grammar/common/pratt/Pratt.hpp"
// #include "parser/grammar/common/type/Type.hpp"

namespace Parser::Common
{
    static inline Pratt Pratt;
    // extern Expr Expr;
    // extern Type Type;

    // extern Block Block;
    // extern IdentifierInitialization IdentifierInitialization;
    // extern Mutability Mutability;
    static inline Terminator Terminator;

} // namespace Parser::Common
