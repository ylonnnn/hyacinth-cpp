#include <cstdint>
#include <cstring>
#include <variant>

#include "core/type/Type.hpp"
#include "core/type/primitive/Float.hpp"

namespace Core
{
    FloatBitWidthType::FloatBitWidthType() : BitWidthType() {}

    bool FloatBitWidthType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::visit(
            [&](const auto &val) -> bool
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, uint64_t>)
                    return val == 8 || val == 16 || val == 32 || val == 64;

                else
                    return false;
            },
            value);
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
        std::vector<uint> bit_widths = {8, 16, 32, 64};
        std::string allowed_bw = "";

        for (const auto &bw : bit_widths)
        {
            auto last = bit_widths.back() == bw;
            if (last)
                allowed_bw += "and ";

            allowed_bw += Diagnostic::NOTE_GEN + std::to_string(bw) +
                          Utils::Styles::Reset + (last ? "" : ", ");
        }

        return std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            "Only the values " + allowed_bw + Utils::Styles::Reset +
                " are accepted.",
            "Implement suggestion here");
    }

    FloatType::FloatType(Core::Environment *environment)
        : BaseType(environment, "float")
    {
        create_parameter("_bw", TypeParameterType::Constant,
                         Type(&bw_type_, {}));
    }

    bool FloatType::can_fit(double value, uint t_exponent,
                            uint t_mantissa) const
    {
        uint64_t bits;
        std::memcpy(&bits, &value, sizeof(bits));

        uint64_t sign = (bits >> 63) & 1;
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
        // int32_t e_bits = man_64b - t_mantissa;
        // uint64_t ro_mantissa = (mantissa + (1ull << (e_bits - 1))) >> e_bits;

        // return (ro_mantissa >> t_mantissa) == 0;

        return (mantissa >> t_mantissa) == 0;
    }

    bool FloatType::assignable(const Core::Value &value,
                               const std::vector<TypeArgument> &arguments) const
    {
        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr = std::get_if<Core::Value>(&arguments[0]))
                if (auto val_ptr = std::get_if<uint64_t>(ptr))
                    bw = *val_ptr;
        }

        return std::visit(
            [&](const auto &val) -> bool
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, double>)
                {
                    std::pair<uint, uint> bit_pairs[] = {
                        {3, 4}, {5, 10}, {8, 23}, {11, 52}};

                    size_t idx = bw == 8 ? 0 : bw == 16 ? 1 : bw == 32 ? 2 : 3;
                    auto &[exponent, mantissa] = bit_pairs[idx];

                    return can_fit(val, exponent, mantissa);
                }

                else
                    return false;
            },
            value);
    }

    bool FloatType::assignable_with(const BaseType &type) const
    {
        return BaseType::assignable_with(type);
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic>
    FloatType::make_suggestion(AST::Node *node,
                               const std::vector<TypeArgument> &arguments) const
    {
        return nullptr;
    }

} // namespace Core
