#include <cassert>
#include <cstddef>
#include <functional>

#include "core/value/Value.hpp"

namespace Core
{
    value_base_type::operator std::string() const { return to_string(); }

    size_t null::hash() const { return std::hash<std::nullptr_t>{}(nullptr); }

    std::string null::to_string() const { return "null"; }

    integer::integer(uint64_t value, bool is_neg)
        : value(value), is_negative(is_neg)
    {
    }

    std::string integer::to_string() const
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

    std::string object::to_string() const
    {
        // TODO: object::to_string()
        // T { [field]: [value] }
        return "{}";
    }

    array::array(InstantiatedType *element_type,
                 std::vector<Value *> &&elements)
        : element_type(element_type), elements_(std::move(elements))
    {
    }

    size_t array::size() const { return elements_.size(); }

    Value *array::get(size_t idx)
    {
        return idx >= size() ? nullptr : elements_[idx];
    }

    const Value *array::get(size_t idx) const
    {
        return const_cast<array *>(this)->get(idx);
    }

    size_t array::hash() const
    {
        size_t hash_val = 0;

        for (auto &el : elements_)
            hash_val += std::hash<Value *>{}(el);

        return hash_val;
    }

    std::string array::to_string() const
    {
        // TODO: array::to_string()
        // []T{...}
        return "{}";
    }

    pointer::pointer(Value *pointee) : pointee(pointee) {}

    size_t pointer::hash() const { return std::hash<Value *>{}(pointee); }

    std::string pointer::to_string() const
    {
        // TODO: pointer::to_string()
        // *T
        return "*_";
    }

    Value::Value(PositionRange *range) : range(range) {}

    ReadValue::ReadValue(std::unique_ptr<T> &&value, InstantiatedType *type,
                         PositionRange *range)
        : Value(range), value(std::move(value)), type(type)
    {
    }

    size_t ReadValue::hash() const
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

    std::string ReadValue::to_string() const
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

    std::ostream &operator<<(std::ostream &os, const ReadValue &val)
    {
        return os << val.to_string();
    }

    LocatorValue::LocatorValue(Value &value, PositionRange &range)
        : Value(&range), value(value)
    {
    }

    ReadValue &get_rvalue(Value &value)
    {
        Value *curr = &value;
        while (curr != nullptr)
        {
            if (typeid(*curr) == typeid(ReadValue))
                return static_cast<ReadValue &>(*curr);

            curr = &static_cast<LocatorValue *>(curr)->value;
        }

        __builtin_unreachable();
    }

    ReadValue *get_rvalue(Value *value)
    {
        return value == nullptr ? nullptr : &get_rvalue(*value);
    }

} // namespace Core
