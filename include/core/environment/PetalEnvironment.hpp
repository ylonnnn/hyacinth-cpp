#pragma once

#include "core/environment/Environment.hpp"

namespace Core
{
    struct PetalSymbol;

    struct PetalEnvironment : Environment
    {
        PetalSymbol *petal = nullptr;

        PetalEnvironment(Environment *parent, PetalSymbol *petal);
        ~PetalEnvironment() = default;

        // Symbol *resolve_symbol(
        //     const std::string &name,
        //     size_t depth = static_cast<size_t>(ResolutionType::Root))
        //     override;

        // BaseType *resolve_type(
        //     const std::string &name,
        //     size_t depth = static_cast<size_t>(ResolutionType::Root))
        //     override;

        // VariableSymbol *resolve_variable(
        //     const std::string &name,
        //     size_t depth = static_cast<size_t>(ResolutionType::Root))
        //     override;
    };

} // namespace Core
