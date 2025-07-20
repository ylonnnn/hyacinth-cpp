#pragma once

#include "parser/grammar/common/Block.hpp"
#include "parser/grammar/common/IdentifierInit.hpp"
#include "parser/grammar/common/Mutability.hpp"
#include "parser/grammar/common/Terminator.hpp"
#include "parser/grammar/common/expr/Expr.hpp"
#include "parser/grammar/common/type/Type.hpp"

namespace Parser::Common
{
    extern Expr Expr;
    extern Type Type;

    extern Block Block;
    extern IdentifierInitialization IdentifierInitialization;
    extern Mutability Mutability;
    extern Terminator Terminator;

} // namespace Parser::Common
