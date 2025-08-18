#include <string>

#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "core/symbol/FunctionSymbol.hpp"
#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

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
                {
                    if constexpr (std::is_same_v<T, char>)
                        return os << std::string({val});

                    return os << std::to_string(val);
                }
            },
            value);
    }

    h_int::h_int(int64_t val, bool is_signed)
        : value_({.signed_ = val}), is_signed_(is_signed)
    {
    }

    int64_t &h_int::i64() { return value_.signed_; }

    int64_t h_int::i64() const { return value_.signed_; }

    uint64_t &h_int::u64() { return value_.unsigned_; }

    uint64_t h_int::u64() const { return value_.unsigned_; }

    bool h_int::is_signed() const { return is_signed_; }

    h_int::operator std::string() const
    {
        if (is_signed_)
            return std::to_string(value_.signed_);

        return std::to_string(value_.unsigned_);
    }

    null::operator std::string() const { return "null"; }

    value_data *object::get(const std::string &key)
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

    bool object::set(const std::string &key, value_data &&value)
    {
        return value_.try_emplace(key, std::move(value)).second;
    }

    size_t object::size() const { return value_.size(); }

    std::unordered_map<std::string_view, value_data> &object::value()
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

            if (value.type != nullptr)
                str += "<" + value.type->to_string() + ">";

            str += std::string(field) + ":" +
                   std::visit(
                       [&](const auto &val) -> std::string
                       {
                           using T = std::decay_t<decltype(val)>;

                           if constexpr (std::is_convertible_v<T, std::string>)
                               return std::string(val);
                           else
                           {
                               if constexpr (std::is_same_v<T, char>)
                                   return std::string({val});

                               return std::to_string(val);
                           }
                       },
                       value.value ? *value.value : Core::null{});

            if (en_c < value_.size())
                str += ",";
        }

        return str + "}";
    }

    array::array(Type *element_type) : element_type_(element_type) {}

    Type *&array::element_type() { return element_type_; }

    const Type *array::element_type() const { return element_type_; }

    std::vector<value_data> &array::elements() { return elements_; }

    const std::vector<value_data> &array::elements() const { return elements_; }

    value_data *array::get(size_t idx)
    {
        return idx >= elements_.size() ? nullptr : &elements_[idx];
    }

    const value_data *array::get(size_t idx) const
    {
        return idx >= elements_.size() ? nullptr : &elements_[idx];
    }

    array::operator std::string() const
    {
        std::string str("{");

        size_t en_c = 0;
        for (const auto &[value, type] : elements_)
        {
            en_c++;

            if (type != nullptr)
                str += "<" + type->to_string() + ">";

            str += std::visit(
                [&](const auto &val) -> std::string
                {
                    using T = std::decay_t<decltype(val)>;

                    if constexpr (std::is_convertible_v<T, std::string>)
                        return std::string(val);
                    else
                    {
                        if constexpr (std::is_same_v<T, char>)
                            return std::string({val});

                        return std::to_string(val);
                    }
                },
                value ? *value : Core::null{});

            if (en_c < elements_.size())
                str += ",";
        }

        return str + "}";
    }

    callable::callable(FunctionSymbol *value) : value_(value) {}

    Value callable::call([[maybe_unused]] std::vector<Value> args)
    {
        AST::FunctionDeclarationStmt *stmt = value_->node;
        if (stmt == nullptr)
            return null{};

        if (stmt->is_definition())
            if ([[maybe_unused]] auto ptr =
                    static_cast<AST::FunctionDefinitionStmt *>(stmt))
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
