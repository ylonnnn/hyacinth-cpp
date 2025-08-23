#pragma once

#include "core/environment/Environment.hpp"

namespace Core
{
    class FunctionEnvironment : public Environment
    {
      private:
        FunctionSymbol *fn_ = nullptr;

      public:
        FunctionEnvironment(Environment *parent, FunctionSymbol *fn);
        ~FunctionEnvironment() = default;

        FunctionSymbol *fn_symbol();
        const FunctionSymbol *fn_symbol() const;
    };

} // namespace Core
