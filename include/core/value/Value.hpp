#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace Core
{
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

    struct integer;
    struct object;
    struct callable;
    // struct array; // ?

    struct integer : value_base_type
    {
        uint64_t value;
        bool is_neg;

        integer(uint64_t value, bool is_neg);

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
        void set(const std::string &key, Value &value);

        Value *get(const std::string &key);
        const Value *get(const std::string &key) const;

        size_t hash() const override;

        operator std::string() const;

      private:
        std::unordered_map<std::string, Value *> entries_;
    };

    struct InstantiatedType;
    struct Value
    {
        using T = std::variant<null, integer, double, bool, char, std::string>;

        std::unique_ptr<T> value;
        InstantiatedType *type = nullptr;

        Value(std::unique_ptr<T> &&value, InstantiatedType *type);

        size_t hash() const;
        std::string to_string() const;

        friend std::ostream &operator<<(std::ostream &os, const Value &val);
    };

    // struct Type;
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

    // struct value_data
    // {
    //     std::shared_ptr<Value> value;
    //     Type *type = nullptr;
    // };

    // struct object
    // {
    //   private:
    //     Type *type_;
    //     std::unordered_map<std::string_view, value_data> value_;

    //   public:
    //     Type *&type();
    //     const Type *type() const;

    //     std::unordered_map<std::string_view, value_data> &value();
    //     const std::unordered_map<std::string_view, value_data> &value()
    //     const;

    //     value_data *get(const std::string &key);
    //     const value_data *get(const std::string &key) const;

    //     bool set(const std::string &key, value_data &&value);

    //     Value *get_value(const std::string &key);
    //     const Value *get_value(const std::string &key) const;
    //     Type *get_type(const std::string &key);
    //     const Type *get_type(const std::string &key) const;

    //     size_t size() const;

    //     operator std::string() const;
    // };

    // struct array
    // {
    //   private:
    //     Type *element_type_ = nullptr;
    //     std::vector<value_data> elements_;

    //   public:
    //     array(Type *element_type);

    //     Type *&element_type();
    //     const Type *element_type() const;

    //     std::vector<value_data> &elements();
    //     const std::vector<value_data> &elements() const;

    //     size_t size() const;

    //     value_data *get(size_t idx);
    //     const value_data *get(size_t idx) const;

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
