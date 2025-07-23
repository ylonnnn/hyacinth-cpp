#include "core/environment/Environment.hpp"

namespace Core
{
    Environment::Environment(Environment *parent) : parent_(parent)
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

    void Environment::declare_type(std::unique_ptr<Type> type)
    {
        std::string name = std::string(type->name());

        auto it = types_.find(name);
        if (it != types_.end())
            return;

        types_.insert_or_assign(name, std::move(type));
        // types_[name] = std::move(type);
    }

    void Environment::declare_symbol(std::unique_ptr<Symbol> symbol)
    {
        auto it = symbols_.find(symbol->name);
        if (it != symbols_.end())
            return;

        symbols_.insert_or_assign(symbol->name, std::move(symbol));
        // symbols_[symbol->name] = std::move(symbol);
    }

    void Environment::declare_variable(std::unique_ptr<VariableSymbol> variable)
    {
        auto it = variables_.find(variable->name);
        if (it != variables_.end())
            return;

        update_variable(variable->name, variable->value);
        declare_symbol(std::move(variable));
    }

    void Environment::update_variable(const std::string &name, Value value)
    {
        variables_.insert_or_assign(name, value);
    }

    Type *Environment::resolve_type(const std::string &name)
    {
        auto it = types_.find(name);
        if (it == types_.end())
            return parent_ == nullptr ? nullptr : parent_->resolve_type(name);

        return it->second.get();
    }

    Symbol *Environment::resolve_symbol(const std::string &name)
    {
        auto it = symbols_.find(name);
        if (it == symbols_.end())
            return parent_ == nullptr ? nullptr : parent_->resolve_symbol(name);

        return it->second.get();
    }

    VariableSymbol *Environment::resolve_variable(const std::string &name)
    {
        auto it = symbols_.find(name);
        if (it == symbols_.end())
            return parent_ == nullptr ? nullptr
                                      : parent_->resolve_variable(name);

        return dynamic_cast<VariableSymbol *>(it->second.get());
    }

} // namespace Core
