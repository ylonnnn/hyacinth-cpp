#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

// #include "core/symbol/Symbol.hpp"
// #include "core/symbol/VariableSymbol.hpp"
#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    namespace EnvironmentResolutionType
    {
        constexpr size_t Current = 1, Parent = 2, Root = SIZE_MAX;
    }

    struct Environment
    {
        Environment *parent = nullptr;
        std::vector<std::unique_ptr<Environment>> children;

        // TypeTable types;
        // std::unordered_map<std::string_view, std::unique_ptr<Symbol>>
        // symbols_; std::unordered_map<std::string_view, Value> variables_;

        Environment(Environment *parent = nullptr);
        virtual ~Environment() = default;

        void initialize();
        void initialize_types();

        template <
            typename T = Environment, typename... Args,
            typename = std::enable_if_t<std::is_base_of_v<Environment, T>>>
        T &create_child(Args... args)
        {
            children.push_back(
                std::make_unique<T>(std::forward<Args>(args)...));

            return *children.back();
        }

        void declare_type(std::unique_ptr<BaseType> &&type);

        virtual BaseType *
        resolve_type(const std::string &name,
                     size_t depth = EnvironmentResolutionType::Root);
        virtual const BaseType *
        resolve_type(const std::string &name,
                     size_t depth = EnvironmentResolutionType::Root) const;

        virtual void print(std::ostream &os, uint32_t tab) const;

      private:
        std::unordered_map<std::string, std::unique_ptr<BaseType>> types_;

        // virtual void display_symbol_table(std::ostream &os, uint32_t tab)
        // const;

        // void declare_symbol(std::unique_ptr<Symbol> symbol);
        // void declare_variable(std::unique_ptr<VariableSymbol> variable);

        // void update_variable(const std::string &name, Value value);

        // TypeResolutionResult resolve_ast_type(
        //     AST::Type &ast_type,
        //     size_t depth = static_cast<size_t>(ResolutionType::Root));

        // virtual BaseType *
        // resolve_type(const std::string &name,
        //              size_t depth =
        //              static_cast<size_t>(ResolutionType::Root));

        // virtual Symbol *resolve_symbol(
        //     const std::string &name,
        //     size_t depth = static_cast<size_t>(ResolutionType::Root));

        // virtual VariableSymbol *resolve_variable(
        //     const std::string &name,
        //     size_t depth = static_cast<size_t>(ResolutionType::Root));
    };

} // namespace Core
