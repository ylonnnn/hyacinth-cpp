#include "core/program/DependencyEnvironment.hpp"
#include "utils/style.hpp"

namespace Core
{
    DependencyEnvironment::DependencyEnvironment() : Environment(nullptr, "Dep")
    {
    }

    std::vector<Environment *> &DependencyEnvironment::dependencies()
    {
        return dependencies_;
    }

    void DependencyEnvironment::display_symbol_table(std::ostream &os,
                                                     uint8_t tab) const
    {
        os << "[" << name_ << "] {";

        for (const auto &dependency : dependencies_)
        {
            os << "\n" << utils::tab(tab, 4);
            dependency->display_symbol_table(os, tab + 1);
        }

        os << "\n" << utils::tab(tab - 1, 4) << "}";
    }

    Symbol *DependencyEnvironment::resolve_symbol(const std::string &name,
                                                  size_t depth)
    {
        if (depth == 0)
            return nullptr;

        Symbol *resolved = Environment::resolve_symbol(name, depth - 1);
        if (resolved != nullptr)
            return resolved;

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

        BaseType *resolved = Environment::resolve_type(name, depth - 1);
        if (resolved != nullptr)
            return resolved;

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

        VariableSymbol *resolved =
            Environment::resolve_variable(name, depth - 1);
        if (resolved != nullptr)
            return resolved;

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
