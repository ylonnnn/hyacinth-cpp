#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Core
{
    struct Type;
    struct FunctionSymbol;

    struct h_int
    {
      private:
        union
        {
            int64_t signed_;
            uint64_t unsigned_;
        } value_;

        bool is_signed_ = true;

      public:
        h_int(int64_t s_val, bool is_signed = true);

        bool is_signed() const;

        int64_t &i64();
        int64_t i64() const;
        uint64_t &u64();
        uint64_t u64() const;

        operator std::string() const;
    };

    struct object;
    struct null
    {
        operator std::string() const;
    };

    using Value =
        std::variant<null, h_int, double, bool, char, std::string, object>;

    std::ostream &operator<<(std::ostream &os, const Value &value);

    struct object_entry;
    struct object
    {
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
        std::shared_ptr<Value> value;
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

} // namespace Core
