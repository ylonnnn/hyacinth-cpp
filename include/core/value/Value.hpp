#pragma once

#include "core/position/Position.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace Core
{
    struct InstantiatedType;

    struct value_base_type
    {
        virtual size_t hash() const = 0;
    };

    struct Value;

    struct null : value_base_type
    {
        size_t hash() const override;

        operator std::string() const;
    };

    struct callable;

    struct integer : value_base_type
    {
        uint64_t value;
        bool is_negative;

        integer(uint64_t value, bool is_negative);

        template <typename T,
                  typename = std::enable_if_t<std::is_same_v<T, int64_t> ||
                                              std::is_same_v<T, uint64_t>>>
        T as()
        {
            if constexpr (std::is_same_v<T, int64_t>)
                return static_cast<int64_t>(value);

            else
                return value;
        }

        size_t hash() const override;

        operator std::string() const;
    };

    struct object : value_base_type
    {
        InstantiatedType *type = nullptr;
        object(InstantiatedType *type);

        void set(const std::string &key, Value &value);

        Value *get(const std::string &key);
        const Value *get(const std::string &key) const;

        size_t hash() const override;

        operator std::string() const;

      private:
        std::unordered_map<std::string, Value *> entries_;
    };

    struct array : value_base_type
    {
        InstantiatedType *element_type = nullptr;

        array(InstantiatedType *element_type);

        size_t size() const;

        Value *get(size_t idx);
        const Value *get(size_t idx) const;

        size_t hash() const;
        operator std::string() const;

      private:
        std::vector<Value *> elements_;
    };

    using character = uint32_t;

    enum class ValueType
    {
        RValue,
        LValue,
    };

    struct Value
    {
        using T = std::variant<null, integer, double, bool, character,
                               std::string, array>;

        std::unique_ptr<T> value;
        InstantiatedType *type = nullptr;
        ValueType val_type = ValueType::RValue;
        Core::PositionRange *range = nullptr;

        Value(std::unique_ptr<T> &&value, InstantiatedType *type,
              ValueType val_type = ValueType::RValue,
              Core::PositionRange *range = nullptr);

        size_t hash() const;
        std::string to_string() const;

        friend std::ostream &operator<<(std::ostream &os, const Value &val);
    };

    // struct InstantiatedType;
    // struct FunctionSymbol;

    // struct h_int
    // {
    //   private:
    //     union
    //     {
    //         int64_t signed_;
    //         uint64_t unsigned_;
    //     } value_;

    //     bool is_signed_ = true;

    //   public:
    //     h_int(int64_t s_val, bool is_signed = true);

    //     bool is_signed() const;

    //     int64_t &i64();
    //     int64_t i64() const;
    //     uint64_t &u64();
    //     uint64_t u64() const;

    //     operator std::string() const;
    // };

    // struct object;
    // struct array;
    // struct null
    // {
    //     operator std::string() const;
    // };

    // using Value = std::variant<null, h_int, double, bool, char, std::string,
    //                            object, array>;

    // std::ostream &operator<<(std::ostream &os, const Value &value);

    // structValue *
    // {
    //     std::shared_ptr<Value> value;
    //     InstantiatedType *type = nullptr;
    // };

    // struct object
    // {
    //   private:
    //     InstantiatedType *type_;
    //     std::unordered_map<std::string_view, Value *> value_;

    //   public:
    //     InstantiatedType *&type();
    //     const InstantiatedType *type() const;

    //     std::unordered_map<std::string_view, Value *> &value();
    //     const std::unordered_map<std::string_view, Value *> &value()
    //     const;

    //     Value * *get(const std::string &key);
    //     const Value * *get(const std::string &key) const;

    //     bool set(const std::string &key, Value * &&value);

    //     Value *get_value(const std::string &key);
    //     const Value *get_value(const std::string &key) const;
    //     InstantiatedType *get_type(const std::string &key);
    //     const InstantiatedType *get_type(const std::string &key) const;

    //     size_t size() const;

    //     operator std::string() const;
    // };

    // struct callable
    // {
    //   private:
    //     FunctionSymbol *value_ = nullptr;

    //   public:
    //     callable(FunctionSymbol *value);

    //     Value call(std::vector<Value> args);

    //     operator std::string() const;
    // };

} // namespace Core
