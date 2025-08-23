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

        void display_symbol_table(std::ostream &os, uint8_t tab) const override;

        Symbol *resolve_symbol(
            const std::string &name,
            size_t depth = static_cast<size_t>(ResolutionType::Root)) override;

        BaseType *resolve_type(
            const std::string &name,
            size_t depth = static_cast<size_t>(ResolutionType::Root)) override;

        VariableSymbol *resolve_variable(
            const std::string &name,
            size_t depth = static_cast<size_t>(ResolutionType::Root)) override;
    };

} // namespace Core
