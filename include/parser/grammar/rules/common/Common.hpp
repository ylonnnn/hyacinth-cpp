#pragma once

#include "parser/grammar/rules/common/IdentifierInit.hpp"
#include "parser/grammar/rules/common/Mutability.hpp"
#include "parser/grammar/rules/common/Terminator.hpp"

namespace Parser::Common
{
    extern IdentifierInitialization IdentifierInitialization;
    extern Mutability Mutability;
    extern Terminator Terminator;

} // namespace Parser::Common
