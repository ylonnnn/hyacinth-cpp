#pragma once

#include "core/environment/Environment.hpp"

namespace Core
{
    class DependencyEnvironment : public Environment
    {
      private:
        //
        std::vector<Environment *> dependencies_;

      public:
        DependencyEnvironment();

        std::vector<Environment *> &dependencies();

        Symbol *resolve_symbol(const std::string &name) override;
        BaseType *resolve_type(const std::string &name) override;
        VariableSymbol *resolve_variable(const std::string &name) override;
    };

} // namespace Core
