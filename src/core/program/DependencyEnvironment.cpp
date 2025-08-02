#include "core/program/DependencyEnvironment.hpp"

namespace Core
{
    DependencyEnvironment::DependencyEnvironment() : Environment(nullptr) {}

    std::vector<Environment *> &DependencyEnvironment::dependencies()
    {
        return dependencies_;
    }

    Symbol *DependencyEnvironment::resolve_symbol(const std::string &name,
                                                  size_t depth)
    {
        if (depth == 0)
            return nullptr;

        for (const auto &dependency : dependencies_)
        {
            Symbol *resolved = dependency->resolve_symbol(name, depth - 1);
            if (!resolved)
                continue;

            if (resolved->accessibility == SymbolAccessibility::Private)
                return nullptr;

            return resolved;
        }

        return nullptr;
    }

    BaseType *DependencyEnvironment::resolve_type(const std::string &name,
                                                  size_t depth)
    {
        if (depth == 0)
            return nullptr;

        for (const auto &dependency : dependencies_)
        {
            BaseType *resolved = dependency->resolve_type(name, depth - 1);
            if (!resolved)
                continue;

            if (resolved->symbol()->accessibility ==
                SymbolAccessibility::Private)
                return nullptr;

            return resolved;
        }

        return nullptr;
    }

    VariableSymbol *
    DependencyEnvironment::resolve_variable(const std::string &name,
                                            size_t depth)
    {
        if (depth == 0)
            return nullptr;

        for (const auto &dependency : dependencies_)
        {
            VariableSymbol *resolved =
                dependency->resolve_variable(name, depth - 1);
            if (!resolved)
                continue;

            if (resolved->accessibility == SymbolAccessibility::Private)
                return nullptr;

            return resolved;
        }

        return nullptr;
    }

} // namespace Core
