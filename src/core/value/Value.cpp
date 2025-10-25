#include "core/value/Value.hpp"
#include <cstddef>
#include <functional>

namespace Core
{
    size_t null::hash() const { return std::hash<std::nullptr_t>{}(nullptr); }

    null::operator std::string() const { return "null"; }

    integer::operator std::string() const
    {
        return std::to_string(is_neg ? static_cast<int64_t>(value) : value);
    }

    size_t integer::hash() const
    {
        return is_neg ? std::hash<int64_t>{}(static_cast<int64_t>(value))
                      : std::hash<uint64_t>{}(value);
    }

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

} // namespace Core
