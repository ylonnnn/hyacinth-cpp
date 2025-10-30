#pragma once

#include "core/environment/Environment.hpp"

namespace Core
{
    struct DependencyEnvironment : Environment
    {
        std::vector<Environment *> dependencies;

        DependencyEnvironment();

        // void display_symbol_table(std::ostream &os, uint8_t tab) const
        // override;

        BaseType *
        resolve_type(const std::string &name,
                     size_t depth = EnvironmentResolutionType::Root) override;
        const BaseType *resolve_type(
            const std::string &name,
            size_t depth = EnvironmentResolutionType::Root) const override;

        Symbol *
        resolve_symbol(const std::string &name,
                       size_t depth = EnvironmentResolutionType::Root) override;
        const Symbol *resolve_symbol(
            const std::string &name,
            size_t depth = EnvironmentResolutionType::Root) const override;
    };

} // namespace Core
