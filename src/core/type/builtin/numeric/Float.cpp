#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <variant>

#include "core/environment/Environment.hpp"
#include "core/type/builtin/bases/NumericBase.hpp"
// #include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"
#include "core/type/builtin/numeric/Float.hpp"
#include "core/value/ValuePool.hpp"

namespace Core
{
    FloatBitWidthType::FloatBitWidthType(Environment &environment)
        : BitWidthType(environment)
    {
    }

    FloatBitWidthType::T *
    FloatBitWidthType::create_instance(std::vector<GenericArgument> &&arguments,
                                       Core::PositionRange *range)
    {
        return TYPE_POOL.add(
            std::make_unique<T>(*this, std::move(arguments), range));
    }

    FloatBitWidthType::Signal
    FloatBitWidthType::assignable(const std::vector<GenericArgument> &arguments,
                                  Value *value, TypeResult &result) const
    {
        auto error = [this, &result, value]() -> auto
        {
            return result.error(
                *value->range, Diagnostic::ErrorType::TypeMismatch,
                "expected value of type '" + std::string(name) +
                    "', received '" + value->type->to_string() + "'.");
        };

        auto ptr = std::get_if<integer>(value->value.get());
        if (value == nullptr || ptr == nullptr)
        {
            error();
            return Mismatch;
        }

        integer &i_val = *ptr;
        auto val = i_val.as<uint64_t>();

        if (val == 8 || val == 16 || val == 32 || val == 64)
            return Assignable;

        error()->add_detail(make_suggestion(arguments, value));
        return Mismatch;
    }

    FloatBitWidthType::T *FloatBitWidthType::infer(Value &value)
    {
        return nullptr;
    }

    std::unique_ptr<Diagnostic::Diagnostic> FloatBitWidthType::make_suggestion(
        const std::vector<GenericArgument> &arguments, Value *value) const
    {
        if (value == nullptr || value->range == nullptr)
            return nullptr;

        return std::make_unique<Diagnostic::Diagnostic>(
            Diagnostic::DiagnosticSeverity::Note,
            static_cast<uint32_t>(Diagnostic::NoteType::Suggestion),
            *value->range, "expects the values 8, 16, 32, and 64.");
    }

    FloatInstantiated::FloatInstantiated(
        BaseType &base, std::vector<GenericArgument> &&arguments,
        Core::PositionRange *range)
        : NumericInstantiated(base, std::move(arguments), range)
    {
    }

    TypeResult FloatInstantiated::assignable(Value *value) const
    {
        return InstantiatedType::assignable(value);
    }

    FloatType ::FloatType(Environment &environment)
        : NumericBase(environment, "float"), bw_type_(environment)
    {
        add_parameter(TypeParameterType::Value, "_Bw",
                      bw_type_.create_instance({}));
    }

    FloatType::~FloatType() = default;

    void FloatType::default_operations() {}

    FloatType::T *
    FloatType::create_instance(std::vector<GenericArgument> &&arguments,
                               Core::PositionRange *range)
    {
        return TYPE_POOL.add(
            std::make_unique<T>(*this, std::move(arguments), range));
    }

    int8_t FloatType::can_fit(Value &value, uint64_t t_exponent,
                              uint64_t t_mantissa) const
    {
        auto ptr = std::get_if<double>(value.value.get());
        if (ptr == nullptr)
            return -2;

        double val = *ptr;
        uint64_t bits;

        std::memcpy(&bits, &val, sizeof(bits));

        // uint64_t sign = (bits >> 63) & 1;
        uint64_t exponent =
            (bits >> 52) & 0x7FF; // Intersect with 2047 for
                                  // 64-bit (double) and 255 for 32-bit (float
                                  // or single precision)
        uint64_t mantissa = bits & 0xFFFFFFFFFFFFF;

        switch (exponent)
        {
            // exponent == 0:
            //      mantissa == 0 -> 0
            //      mantissa != 0 -> Subnormal
            case 0:
                return mantissa ? -1 : 0;

            // exponent == 0x7FF // (2047) double 11-bits
            //      mantissa == 0 -> Infinite
            //      mantissa != 0 -> NaN
            case 0x7FF:
                return mantissa == 0 ? 1 : -2;
        }

        constexpr uint8_t exp_64b = 11, man_64b = 52;
        constexpr uint64_t bias = (1ull << (exp_64b - 1)) - 1;

        int64_t actual_exp = exponent - bias;

        uint64_t t_bias = (1ull << (t_exponent - 1)) - 1;
        int64_t max_exp = t_bias, min_exp = -(t_bias + 1);

        // Overlaps target exponent range
        if (actual_exp < min_exp)
            return -1;
        if (actual_exp > max_exp)
            return 1;

        // Rounding
        int32_t e_bits = man_64b - t_mantissa;
        uint64_t ro_mantissa = (mantissa + (1ull << (e_bits - 1))) >> e_bits;

        // return (mantissa >> t_mantissa) == 0;
        return (ro_mantissa >> t_mantissa) == 0 ? 0 : 0;
    }

    FloatType::Signal
    FloatType::assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const
    {
        if (value == nullptr)
            return Mismatch;

        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr = std::get_if<Value *>(&arguments[0]))
                if (auto val_ptr = std::get_if<integer>((*ptr)->value.get()))
                    bw = val_ptr->as<uint64_t>();
        }

        auto [exponent, mantissa] = bit_pairs_.back();
        for (auto &[exp, mant] : bit_pairs_)
            if (bw == 1 + exp + mant)
                exponent = exp, mantissa = mant;

        switch (can_fit(*value, exponent, mantissa))
        {
            case -1:
                return Underflow;
            case 1:
                return Overflow;
            case -2:
                return Mismatch;
        }

        return Assignable;
    }

    FloatType::T *FloatType::infer(Value &value)
    {
        auto ptr = std::get_if<double>(value.value.get());
        if (ptr == nullptr)
            return nullptr;

        uint32_t bw = bit_width_of(value).first;

        return create_instance(
            {create_value(std::make_unique<Value::T>(Core::integer(bw, false)),
                          bw_type_.create_instance({}))},
            value.range);
    }

    std::unique_ptr<Diagnostic::Diagnostic>
    FloatType::make_suggestion(const std::vector<GenericArgument> &arguments,
                               Value *value) const
    {
        if (value == nullptr || value->range == nullptr)
            return nullptr;

        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr = std::get_if<Value *>(&arguments[0]))
                if (auto val_ptr = std::get_if<integer>((*ptr)->value.get()))
                    bw = val_ptr->as<uint64_t>();
        }

        return nullptr;
        // return std::make_unique<Diagnostic::Diagnostic>(
        //     Diagnostic::DiagnosticSeverity::Note,
        //     static_cast<uint32_t>(Diagnostic::NoteType::Suggestion),
        //     Core::PositionRange(*value->range),
        //     "expects values within " + std::to_string(min) + " to " +
        //         std::to_string(max) + ".");
    }

    std::pair<uint32_t, std::pair<uint64_t, uint64_t>>
    FloatType::bit_width_of(Value &value) const
    {
        uint32_t default_bw = 64;

        for (auto &[exponent, mantissa] : bit_pairs_)
            if (can_fit(value, exponent, mantissa) == 0)
                return {static_cast<uint32_t>(1 + exponent + mantissa),
                        {exponent, mantissa}};

        return {default_bw, bit_pairs_.back()};
    };

    // void FloatType::default_operations()
    // {
    //     using namespace Lexer::TokenTypes;
    //     using namespace Operator;

    //     // Arithmetic
    //     auto __h_bw = [&](Type *left, Type *right) -> Type *
    //     { return higher_bit_width(left, right); };

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Arithmetic::Plus, float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> double
    //              { return a + b; }, __h_bw)

    //         });

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Arithmetic::Minus, float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> double
    //              { return a - b; }, __h_bw)});

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Arithmetic::Multiplication, float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> double
    //              { return a * b; }, __h_bw)});

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Arithmetic::Division, float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> double
    //              { return a / b; }, __h_bw)});

    //     // Relational
    //     auto __rel = [&]([[maybe_unused]] Type *left,
    //                      [[maybe_unused]] Type *right) -> Type *
    //     { return bool_w_; };

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Relational::Equal, float_w_,
    //         float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> bool
    //              { return a == b; }, __rel)});

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Relational::NotEqual, float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> bool
    //              { return a != b; }, __rel)});

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Relational::LessThan, float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> bool { return a < b;
    //              },
    //              __rel)});

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Relational::LessThanEqual, float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> bool
    //              { return a <= b; }, __rel)});

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Relational::GreaterThan, float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> bool { return a > b;
    //              },
    //              __rel)});

    //     Operation::overload_binary(
    //         {Operation::BinarySignature{Relational::GreaterThanEqual,
    //         float_w_,
    //                                     float_w_},
    //          true,
    //          overload<double, double>(
    //              [](const double &a, const double &b) -> bool
    //              { return a >= b; }, __rel)});
    // }

} // namespace Core
