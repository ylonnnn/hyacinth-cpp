#pragma once

#include "core/environment/Environment.hpp"

namespace Core
{
    struct LibSymbol;

    class LibEnvironment : public Environment
    {
      private:
        LibSymbol *lib_ = nullptr;

      public:
        LibEnvironment(Environment *parent, LibSymbol *lib);
        ~LibEnvironment() = default;

        LibSymbol *lib_symbol();
        const LibSymbol *lib_symbol() const;

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
