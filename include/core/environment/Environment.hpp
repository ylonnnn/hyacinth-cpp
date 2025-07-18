#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "core/symbol/Symbol.hpp"
#include "core/symbol/VariableSymbol.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    class Environment
    {
      private:
        Environment *parent_;
        std::vector<std::unique_ptr<Environment>> children_;

        std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols_;
        std::unordered_map<std::string, Value> variables_;
        TypeTable types_;

      public:
        Environment(Environment *parent);

        std::vector<std::unique_ptr<Environment>> &children();
        Environment &create_child();

        void declare_symbol(std::unique_ptr<Symbol> symbol);
        void declare_variable(std::unique_ptr<VariableSymbol> variable);
        void declare_type(std::unique_ptr<Type> type);

        void update_variable(const std::string &name, Value value);

        Symbol *resolve_symbol(const std::string &name);
        Value *resolve_variable(const std::string &name);
        Type *resolve_type(const std::string &name);
    };

} // namespace Core
