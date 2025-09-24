#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <variant>

#include "core/environment/Environment.hpp"
#include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"
#include "core/type/primitive/Float.hpp"

namespace Core
{
    FloatBitWidthType::FloatBitWidthType() : BitWidthType() {}

    bool FloatBitWidthType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        auto ptr = std::get_if<Core::h_int>(&value);
        if (ptr == nullptr || (ptr->is_signed() && ptr->i64() < 0))
            return false;

        uint64_t &val = const_cast<Core::h_int *>(ptr)->u64();

        return val == 8 || val == 16 || val == 32 || val == 64;
    }

    bool FloatBitWidthType::assignable_with(
        [[maybe_unused]] const BaseType &type) const
    {
        return false;
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic>
    FloatBitWidthType::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        std::vector<uint64_t> bit_widths = {8, 16, 32, 64};
        std::string allowed_bw = "";

        for (const auto &bw : bit_widths)
        {
            auto last = bit_widths.back() == bw;
            if (last)
                allowed_bw += "and ";

            allowed_bw += Diagnostic::NOTE_GEN + std::to_string(bw) +
                          utils::Styles::Reset + (last ? "" : ", ");
        }

        return std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            "Only the values " + allowed_bw + utils::Styles::Reset +
                " are accepted.",
            "Implement suggestion here");
    }

    bool FloatType::Wrapper::assignable_with(const Type &type) const
    {
        if (arguments.empty() || type.arguments.empty())
            return this->type->assignable_with(*type.type);

        const TypeArgument &bw = type.arguments[0], &bw_ = arguments[0];

        return Type::assignable_with(type) &&
               std::visit(
                   [&](const auto &val, const auto &val_) -> bool
                   {
                       using Ty = std::decay_t<decltype(val)>;
                       using Ty_ = std::decay_t<decltype(val_)>;

                       if constexpr (std::is_same_v<Ty, Core::Value> &&
                                     std::is_same_v<Ty_, Core::Value>)

                           return std::visit(
                               [&](const auto &v, const auto &v_) -> bool
                               {
                                   using T = std::decay_t<decltype(v)>;
                                   using T_ = std::decay_t<decltype(v_)>;

                                   if constexpr (std::is_same_v<T,
                                                                Core::h_int> &&
                                                 std::is_same_v<T_,
                                                                Core::h_int>)
                                       return v.u64() <= v_.u64();

                                   else
                                       return false;
                               },
                               val, val_);

                       else
                           return false;
                   },
                   bw, bw_);
    }

    FloatType::FloatType(Core::Environment *environment)
        : NumericBase(environment, "float"),
          float_w_(
              dynamic_cast<Wrapper *>(Type::get_or_create<Wrapper>(this, {}))),
          bool_w_(Type::get_or_create(environment_->resolve_type("bool"), {}))
    {
        builtin_ = true;

        add_parameter(TypeParameter{"_Bw", TypeParameterType::Constant,
                                    Type::get_or_create(&bw_type_, {})});

        default_operations();
    }

    void FloatType::default_operations()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        // Arithmetic
        auto __h_bw = [&](Type *left, Type *right) -> Type *
        { return higher_bit_width(left, right); };

        Operation::overload_binary(
            {Operation::BinarySignature{Arithmetic::Plus, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> double
                 { return a + b; }, __h_bw)

            });

        Operation::overload_binary(
            {Operation::BinarySignature{Arithmetic::Minus, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> double
                 { return a - b; }, __h_bw)});

        Operation::overload_binary(
            {Operation::BinarySignature{Arithmetic::Multiplication, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> double
                 { return a * b; }, __h_bw)});

        Operation::overload_binary(
            {Operation::BinarySignature{Arithmetic::Division, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> double
                 { return a / b; }, __h_bw)});

        // Relational
        auto __rel = [&]([[maybe_unused]] Type *left,
                         [[maybe_unused]] Type *right) -> Type *
        { return bool_w_; };

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::Equal, float_w_, float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> bool
                 { return a == b; }, __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::NotEqual, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> bool
                 { return a != b; }, __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::LessThan, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> bool { return a < b; },
                 __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::LessThanEqual, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> bool
                 { return a <= b; }, __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::GreaterThan, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> bool { return a > b; },
                 __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::GreaterThanEqual, float_w_,
                                        float_w_},
             true,
             overload<double, double>(
                 [](const double &a, const double &b) -> bool
                 { return a >= b; }, __rel)});
    }

    bool FloatType::can_fit(double value, uint64_t t_exponent,
                            uint64_t t_mantissa) const
    {
        uint64_t bits;
        std::memcpy(&bits, &value, sizeof(bits));

        // uint64_t sign = (bits >> 63) & 1;
        uint64_t exponent =
            (bits >> 52) & 0x7FF; // Intersect with 2047 for 64-bit (double) and
                                  // 255 for 32-bit (float)
        uint64_t mantissa = bits & 0xFFFFFFFFFFFFF;

        switch (exponent)
        {
            // exponent == 0:
            //      mantissa == 0 -> 0
            //      mantissa != 0 -> Subnormal
            case 0:
                return !mantissa;

            // exponent == 0x7FF // (2047) double 11-bits
            //      mantissa == 0 -> Infinite
            //      mantissa != 0 -> NaN
            case 0x7FF:
                return false;
        }

        constexpr uint8_t exp_64b = 11, man_64b = 52;
        constexpr uint64_t bias = (1ull << (exp_64b - 1)) - 1;

        int64_t actual_exp = exponent - bias;

        uint64_t t_bias = (1ull << (t_exponent - 1)) - 1;
        int64_t max_exp = t_bias, min_exp = -(t_bias + 1);

        // Overlaps target exponent range
        if (actual_exp < min_exp || actual_exp > max_exp)
            return false;

        // Rounding
        int32_t e_bits = man_64b - t_mantissa;
        uint64_t ro_mantissa = (mantissa + (1ull << (e_bits - 1))) >> e_bits;

        return (ro_mantissa >> t_mantissa) == 0;

        // return (mantissa >> t_mantissa) == 0;
    }

    bool FloatType::assignable(const Core::Value &value,
                               const std::vector<TypeArgument> &arguments) const
    {
        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr =
                    std::get_if<std::shared_ptr<Core::Value>>(&arguments[0]))
                if (auto val_ptr = std::get_if<Core::h_int>(&**ptr))
                    if (!val_ptr->is_signed() || val_ptr->i64() > -1)
                        bw = val_ptr->u64();
        }

        return std::visit(
            [&](auto &val) -> bool
            {
                using T = std::decay_t<decltype(val)>;
                constexpr bool is_int = std::is_same_v<T, Core::h_int>;

                if constexpr (std::is_same_v<T, double> || is_int)
                {
                    std::pair<uint64_t, uint64_t> bit_pairs[] = {
                        {3, 4}, {5, 10}, {8, 23}, {11, 52}};

                    size_t idx = bw == 8 ? 0 : bw == 16 ? 1 : bw == 32 ? 2 : 3;
                    auto &[exponent, mantissa] = bit_pairs[idx];

                    if constexpr (is_int)
                        return can_fit(val.is_signed() ? val.i64() : val.u64(),
                                       exponent, mantissa);
                    else
                        return can_fit(val, exponent, mantissa);
                }

                else
                    return false;
            },
            value);
    }

    Type *
    FloatType::construct_wrapper(std::vector<TypeArgument> &&arguments) const
    {
        return Type::get_or_create<Wrapper>(const_cast<FloatType *>(this),
                                            std::move(arguments));
    }

    Type *FloatType::construct_wrapper(uint8_t bit_width) const
    {
        return Type::get_or_create<Wrapper>(
            const_cast<FloatType *>(this),
            {std::make_shared<Core::Value>(bit_width)});
    }

    Type *FloatType::from_value(const Core::Value &value) const
    {
        return std::visit(
            [&](const auto &val) -> Type *
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, double>)
                {
                    std::array<std::pair<uint64_t, uint64_t>, 4> bit_pairs = {
                        std::pair{3, 4}, {5, 10}, {8, 23}, {11, 52}};

                    for (const auto &[exponent, mantissa] : bit_pairs)
                    {
                        if (can_fit(val, exponent, mantissa))
                            return construct_wrapper(exponent + mantissa + 1);
                    }

                    return nullptr;
                }

                else
                    return nullptr;
            },
            value);
    }

    bool FloatType::assignable_with(const BaseType &type) const
    {
        return BaseType::assignable_with(type);
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> FloatType::make_suggestion(
        [[maybe_unused]] AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return nullptr;
    }

} // namespace Core
