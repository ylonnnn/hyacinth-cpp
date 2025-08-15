#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "core/symbol/Symbol.hpp"
#include "core/symbol/VariableSymbol.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    enum class ResolutionType
    {
        Current = 1,
        Parent = 2,
        Root = -1,
    };

    class Environment
    {
      protected:
        Environment *parent_ = nullptr;
        std::string name_;
        std::vector<std::unique_ptr<Environment>> children_;

        TypeTable types_;
        std::unordered_map<std::string_view, std::unique_ptr<Symbol>> symbols_;
        std::unordered_map<std::string_view, Value> variables_;

      public:
        Environment(Environment *parent, const std::string &name = "Env");
        virtual ~Environment() = default;

        Environment *parent();

        std::vector<std::unique_ptr<Environment>> &children();

        Environment &create_child();

        void display_symbol_table(std::ostream &os, uint8_t tab) const;

        void declare_type(std::unique_ptr<BaseType> type);
        void declare_symbol(std::unique_ptr<Symbol> symbol);
        void declare_variable(std::unique_ptr<VariableSymbol> variable);

        void update_variable(const std::string &name, Value value);

        virtual BaseType *
        resolve_type(const std::string &name,
                     size_t depth = static_cast<size_t>(ResolutionType::Root));

        virtual Symbol *resolve_symbol(
            const std::string &name,
            size_t depth = static_cast<size_t>(ResolutionType::Root));

        virtual VariableSymbol *resolve_variable(
            const std::string &name,
            size_t depth = static_cast<size_t>(ResolutionType::Root));
    };

} // namespace Core
