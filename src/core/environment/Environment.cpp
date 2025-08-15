#include "core/environment/Environment.hpp"
#include "utils/style.hpp"

namespace Core
{
    Environment::Environment(Environment *parent, const std::string &name)
        : parent_(parent), name_(std::move(name))
    {
        children_.reserve(8);

        types_.reserve(16);
        symbols_.reserve(16);
        variables_.reserve(16);
    }

    Environment *Environment::parent() { return parent_; }

    std::vector<std::unique_ptr<Environment>> &Environment::children()
    {
        return children_;
    }

    Environment &Environment::create_child()
    {
        children_.emplace_back(std::make_unique<Environment>(this));

        return *children_.back();
    }

    void Environment::display_symbol_table(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "[" << name_ << "] {";

        for (const auto &[_, symbol] : symbols_)
        {
            os << "\n" << inner_indentation;
            symbol->print(os, tab + 1);
        }

        for (const auto &child : children_)
        {
            os << "\n" << inner_indentation;
            child->display_symbol_table(os, tab + 1);
        }

        os << "\n" << indentation << "}";
    }

    void Environment::declare_type(std::unique_ptr<BaseType> type)
    {
        auto it = types_.find(type->name());
        if (it == types_.end())
            types_.emplace(type->name(), std::move(type));
        else
        {
            auto _ = type.release();
            (void)_;
        }
    }

    void Environment::declare_symbol(std::unique_ptr<Symbol> symbol)
    {
        symbols_.insert_or_assign(symbol->name, std::move(symbol));
    }

    void Environment::declare_variable(std::unique_ptr<VariableSymbol> variable)
    {
        if (variable->value)
            variables_.try_emplace(variable->name, *variable->value);

        declare_symbol(std::move(variable));
    }

    void Environment::update_variable(const std::string &name, Value value)
    {
        variables_.insert_or_assign(name, value);
    }

    BaseType *Environment::resolve_type(const std::string &name, size_t depth)
    {
        if (depth == 0)
            return nullptr;

        auto it = types_.find(name);
        if (it == types_.end())
            return parent_ == nullptr ? nullptr
                                      : parent_->resolve_type(name, depth - 1);

        return it->second.get();
    }

    Symbol *Environment::resolve_symbol(const std::string &name, size_t depth)
    {
        if (depth == 0)
            return nullptr;

        auto it = symbols_.find(name);
        if (it == symbols_.end())
            return parent_ == nullptr
                       ? nullptr
                       : parent_->resolve_symbol(name, depth - 1);

        return it->second.get();
    }

    VariableSymbol *Environment::resolve_variable(const std::string &name,
                                                  size_t depth)
    {
        if (depth == 0)
            return nullptr;

        auto it = symbols_.find(name);
        if (it == symbols_.end())
            return parent_ == nullptr
                       ? nullptr
                       : parent_->resolve_variable(name, depth - 1);

        return dynamic_cast<VariableSymbol *>(it->second.get());
    }

} // namespace Core
