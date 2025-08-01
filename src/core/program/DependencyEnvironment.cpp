#include "core/program/DependencyEnvironment.hpp"

namespace Core
{
    DependencyEnvironment::DependencyEnvironment() : Environment(nullptr) {}

    std::vector<Environment *> &DependencyEnvironment::dependencies()
    {
        return dependencies_;
    }

    Symbol *DependencyEnvironment::resolve_symbol(const std::string &name)
    {
        for (const auto &dependency : dependencies_)
        {
            Symbol *resolved = dependency->resolve_symbol(name);
            if (!resolved)
                continue;

            if (resolved->accessibility == SymbolAccessibility::Private)
                continue;

            return resolved;
        }

        return nullptr;
    }

    BaseType *DependencyEnvironment::resolve_type(const std::string &name)
    {
        for (const auto &dependency : dependencies_)
        {
            BaseType *resolved = dependency->resolve_type(name);
            if (!resolved)
                continue;

            return resolved;
        }

        return nullptr;
    }

    VariableSymbol *
    DependencyEnvironment::resolve_variable(const std::string &name)
    {
        for (const auto &dependency : dependencies_)
        {
            VariableSymbol *resolved = dependency->resolve_variable(name);
            if (!resolved)
                continue;

            if (resolved->accessibility == SymbolAccessibility::Private)
                continue;

            return resolved;
        }

        return nullptr;
    }

} // namespace Core
