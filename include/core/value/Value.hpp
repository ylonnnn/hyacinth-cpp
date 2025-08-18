#pragma once

#include <cstdint>
#include <memory>
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
    struct array;
    struct null
    {
        operator std::string() const;
    };

    using Value = std::variant<null, h_int, double, bool, char, std::string,
                               object, array>;

    std::ostream &operator<<(std::ostream &os, const Value &value);

    struct value_data
    {
        std::shared_ptr<Value> value;
        Type *type = nullptr;
    };

    struct object
    {
      private:
        std::unordered_map<std::string_view, value_data> value_;

      public:
        value_data *get(const std::string &key);
        bool set(const std::string &key, value_data &&value);

        Value *get_value(const std::string &key);
        Type *get_type(const std::string &key);

        size_t size() const;

        std::unordered_map<std::string_view, value_data> &value();

        operator std::string() const;
    };

    struct array
    {
      private:
        Type *element_type_ = nullptr;
        std::vector<value_data> elements_;

      public:
        array(Type *element_type);

        Type *&element_type();
        const Type *element_type() const;

        std::vector<value_data> &elements();
        const std::vector<value_data> &elements() const;

        value_data *get(size_t idx);
        const value_data *get(size_t idx) const;

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

} // namespace Core
