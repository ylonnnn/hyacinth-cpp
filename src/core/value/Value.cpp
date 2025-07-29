#include "core/value/Value.hpp"
#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "core/symbol/FunctionSymbol.hpp"
#include "core/type/Type.hpp"
#include <string>

namespace Core
{
    std::ostream &operator<<(std::ostream &os, const Value &value)
    {
        return std::visit(
            [&](const auto &val) -> std::ostream &
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_convertible_v<T, std::string>)
                    return os << std::string(val);
                else
                    return os << std::to_string(val);
            },
            value);
    }

    null::operator std::string() const { return "null"; }

    object_entry *object::get(const std::string &key)
    {
        auto it = value_.find(key);
        if (it == value_.end())
            return nullptr;

        return &it->second;
    }

    Value *object::get_value(const std::string &key)
    {
        auto it = value_.find(key);
        if (it == value_.end())
            return nullptr;

        auto &entry = it->second;
        return entry.value ? &*entry.value : nullptr;
    }

    Type *object::get_type(const std::string &key)
    {
        auto it = value_.find(key);
        if (it == value_.end())
            return nullptr;

        return it->second.type;
    }

    bool object::set(const std::string &key, object_entry &&value)
    {
        return value_.try_emplace(key, std::move(value)).second;
    }

    size_t object::size() const { return value_.size(); }

    std::unordered_map<std::string_view, object_entry> &object::value()
    {
        return value_;
    }

    object::operator std::string() const
    {
        std::string str("{");

        size_t en_c = 0;
        for (const auto &[field, value] : value_)
        {
            en_c++;

            str += std::string(field) + ":" +
                   std::visit(
                       [&](const auto &val) -> std::string
                       {
                           using T = std::decay_t<decltype(val)>;

                           if constexpr (std::is_convertible_v<T, std::string>)
                               return std::string(val);
                           else
                               return std::to_string(val);
                       },
                       value.value ? *value.value : Core::null{});

            if (value.type != nullptr)
                str += "[" + value.type->to_string() + "]";

            if (en_c < value_.size())
                str += ",";
        }

        return str + "}";
    }

    callable::callable(FunctionSymbol *value) : value_(value) {}

    Value callable::call(std::vector<Value> args)
    {
        AST::FunctionDeclarationStmt *stmt = value_->node;
        if (stmt == nullptr)
            return null{};

        if (stmt->is_definition())
            if (auto ptr = static_cast<AST::FunctionDefinitionStmt *>(stmt))
            {
                // TODO: Call Function Symbol
                // ptr->body();
            }

        return null{};
    }

    callable::operator std::string() const
    {
        // TODO: callable operator std::string
        return "";
    }

} // namespace Core
