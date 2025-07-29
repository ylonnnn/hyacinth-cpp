#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Core
{
    struct Type;
    struct FunctionSymbol;

    struct object;
    struct null
    {
        operator std::string() const;
    };

    using Value = std::variant<null, int64_t, uint64_t, double, bool, char,
                               std::string, object>;

    std::ostream &operator<<(std::ostream &os, const Value &value);

    struct object_entry;
    struct object
    {
      private:
      private:
        std::unordered_map<std::string_view, object_entry> value_;

      public:
        object_entry *get(const std::string &key);
        bool set(const std::string &key, object_entry &&value);

        Value *get_value(const std::string &key);
        Type *get_type(const std::string &key);

        size_t size() const;

        std::unordered_map<std::string_view, object_entry> &value();

        operator std::string() const;
    };

    struct object_entry
    {
        std::optional<Value> value = std::nullopt;
        Type *type = nullptr;
    };

    struct callable
    {
      private:
        FunctionSymbol *value_ = nullptr;

      public:
        callable(FunctionSymbol *value);

        Value call(std::vector<Value> args);

        operator std::string() const;
    };

    template <typename T> Value make_value(T raw)
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int64_t>)
            return int64_t(raw);

        else if constexpr (std::is_same_v<T, unsigned int> ||
                           std::is_same_v<T, uint64_t>)
            return uint64_t(raw);

        return raw;
    }

} // namespace Core
