#include "core/program/DependencyEnvironment.hpp"
#include "utils/style.hpp"

namespace Core
{
    DependencyEnvironment::DependencyEnvironment() : Environment(nullptr) {}

    // void DependencyEnvironment::display_symbol_table(std::ostream &os,
    //                                                  uint8_t tab) const
    // {
    //     os << "[" << name_ << "] {";

    //     for (const auto &dependency : dependencies_)
    //     {
    //         os << "\n" << utils::tab(tab, 4);
    //         dependency->display_symbol_table(os, tab + 1);
    //     }

    //     os << "\n" << utils::tab(tab - 1, 4) << "}";
    // }

    BaseType *DependencyEnvironment::resolve_type(const std::string &name,
                                                  size_t depth)
    {
        if (depth == 0)
            return nullptr;

        BaseType *resolved = Environment::resolve_type(name, depth - 1);
        if (resolved != nullptr)
            return resolved;

        for (const auto &dependency : dependencies)
        {
            BaseType *resolved = dependency->resolve_type(name, depth - 1);
            if (!resolved)
                continue;

            // if (resolved->symbol->accessibility ==
            //     SymbolAccessibility::Private)
            //     return nullptr;

            return resolved;
        }

        return nullptr;
    }

    const BaseType *DependencyEnvironment::resolve_type(const std::string &name,
                                                        size_t depth) const
    {
        return const_cast<DependencyEnvironment *>(this)->resolve_type(name,
                                                                       depth);
    }

    Symbol *DependencyEnvironment::resolve_symbol(const std::string &name,
                                                  size_t depth)
    {
        if (depth == 0)
            return nullptr;

        Symbol *resolved = Environment::resolve_symbol(name, depth - 1);
        if (resolved != nullptr)
            return resolved;

        for (const auto &dependency : dependencies)
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

    const Symbol *DependencyEnvironment::resolve_symbol(const std::string &name,
                                                        size_t depth) const
    {
        return const_cast<DependencyEnvironment *>(this)->resolve_symbol(name,
                                                                         depth);
    }

} // namespace Core
