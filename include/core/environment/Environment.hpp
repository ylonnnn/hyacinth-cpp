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

        TypeTable types_;
        std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols_;
        std::unordered_map<std::string, Value> variables_;

      public:
        Environment(Environment *parent);

        Environment *parent();

        std::vector<std::unique_ptr<Environment>> &children();
        Environment &create_child();

        void declare_type(std::unique_ptr<BaseType> type);
        void declare_symbol(std::unique_ptr<Symbol> symbol);
        void declare_variable(std::unique_ptr<VariableSymbol> variable);

        void update_variable(const std::string &name, Value value);

        BaseType *resolve_type(const std::string &name);
        Symbol *resolve_symbol(const std::string &name);
        VariableSymbol *resolve_variable(const std::string &name);
    };

} // namespace Core
