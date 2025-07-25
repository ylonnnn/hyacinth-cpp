#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Core
{
    struct FunctionSymbol;
    struct object;

    struct null
    {
        operator std::string() const;
    };

    using Value = std::variant<null, int64_t, uint64_t, double, bool, char,
                               std::string, object>;

    struct object
    {
      private:
        std::unordered_map<std::string, Value> value_;

      public:
        Value *get(const std::string &key);
        void set(const std::string &key, Value);

        operator std::string() const;
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
