#include <cstddef>
#include <functional>

#include "core/value/Value.hpp"

namespace Core
{
    size_t null::hash() const { return std::hash<std::nullptr_t>{}(nullptr); }

    null::operator std::string() const { return "null"; }

    integer::integer(uint64_t value, bool is_neg)
        : value(value), is_negative(is_neg)
    {
    }

    integer::operator std::string() const
    {
        return std::to_string(is_negative ? static_cast<int64_t>(value)
                                          : value);
    }

    size_t integer::hash() const
    {
        return is_negative ? std::hash<int64_t>{}(static_cast<int64_t>(value))
                           : std::hash<uint64_t>{}(value);
    }

    object::object(InstantiatedType *type) : type(type) {}

    void object::set(const std::string &key, Value &value)
    {
        entries_.insert_or_assign(key, &value);
    }

    Value *object::get(const std::string &key)
    {
        auto it = entries_.find(key);
        return it == entries_.end() ? nullptr : it->second;
    }

    const Value *object::get(const std::string &key) const
    {
        auto it = entries_.find(key);
        return it == entries_.end() ? nullptr : it->second;
    }

    size_t object::hash() const
    {
        size_t hash_val = 0;

        for (const auto &[key, value] : entries_)
            hash_val +=
                std::hash<std::string>{}(key) + std::hash<Value *>{}(value);

        return hash_val;
    }

    object::operator std::string() const
    {
        // TODO: object::operator std::string()
        return "{}";
    }

    Value::Value(std::unique_ptr<Value::T> &&value, InstantiatedType *type,
                 ValueType val_type, Core::PositionRange *range)
        : value(std::move(value)), type(type), val_type(val_type), range(range)
    {
    }

    size_t Value::hash() const
    {
        return std::visit(
            [](auto &val) -> size_t
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_base_of_v<value_base_type, T>)
                    return val.hash();
                else
                    return std::hash<T>{}(val);
            },
            *value);
    }

    std::string Value::to_string() const
    {
        return std::visit(
            [](auto &val) -> std::string
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_convertible_v<T, std::string>)
                    return std::string(val);
                else
                    return std::to_string(val);
            },
            *value);
    }

    std::ostream &operator<<(std::ostream &os, const Value &val)
    {
        return os << val.to_string();
    }

} // namespace Core
