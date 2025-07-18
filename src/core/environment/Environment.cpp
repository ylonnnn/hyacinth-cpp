#include "core/environment/Environment.hpp"

namespace Core
{
    Environment::Environment(Environment *parent) : parent_(parent) {}

    std::vector<std::unique_ptr<Environment>> &Environment::children()
    {
        return children_;
    }

    Environment &Environment::create_child()
    {
        auto child = std::make_unique<Environment>(this);
        Environment &env_ref = *child;

        children_.push_back(std::move(child));

        return env_ref;
    }

    void Environment::declare_symbol(std::unique_ptr<Symbol> symbol)
    {
        auto it = symbols_.find(symbol->name);
        if (it == symbols_.end())
            return;

        symbols_[symbol->name] = std::move(symbol);
    }

    void Environment::declare_variable(std::unique_ptr<VariableSymbol> variable)
    {
        auto it = variables_.find(variable->name);
        if (it != variables_.end())
            return; // Variable already exists

        update_variable(variable->name, variable->value);
        declare_symbol(std::move(variable));
    }

    void Environment::declare_type(std::unique_ptr<Type> type)
    {
        std::string name = std::string(type->name());

        auto it = types_.find(name);
        if (it != types_.end())
            return;

        types_[name] = std::move(type);
    }

    void Environment::update_variable(const std::string &name, Value value)
    {
        variables_.insert_or_assign(name, value);
    }

    Symbol *Environment::resolve_symbol(const std::string &name)
    {
        auto it = symbols_.find(name);
        if (it == symbols_.end())
            return parent_ == nullptr ? nullptr : parent_->resolve_symbol(name);

        return it->second.get();
    }

    Value *Environment::resolve_variable(const std::string &name)
    {
        auto it = variables_.find(name);
        if (it == variables_.end())
            return parent_ == nullptr ? nullptr
                                      : parent_->resolve_variable(name);

        return &it->second;
    }

    Type *Environment::resolve_type(const std::string &name)
    {
        auto it = types_.find(name);
        if (it == types_.end())
            return parent_ == nullptr ? nullptr : parent_->resolve_type(name);

        return it->second.get();
    }

} // namespace Core
