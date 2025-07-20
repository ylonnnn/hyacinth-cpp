#include <cmath>
#include <iostream>
#include <type_traits>
#include <variant>

#include "core/type/Type.hpp"
#include "core/type/primitive/Integer.hpp"
#include "diagnostic/NoteDiagnostic.hpp"

namespace Core
{
    BitWidthType::BitWidthType() : Type(nullptr, "BitWidth") {}

    bool BitWidthType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::visit(
            [&](const auto &val) -> bool
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, uint64_t>)
                {
                    // constexpr bool val_signed = std::is_signed_v<T>;
                    uint8_t n_val = static_cast<uint8_t>(val);

                    return n_val > 0 && n_val <= 64;
                }

                else
                    return false;
            },
            value);
    }

    bool BitWidthType::assignable_with([[maybe_unused]] const Type &type) const
    {
        return false;
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> BitWidthType::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            std::string("Only values within ") + Diagnostic::NOTE_GEN +
                std::to_string(1) + Utils::Styles::Reset + " to " +
                Diagnostic::NOTE_GEN + std::to_string(64) +
                Utils::Styles::Reset + " are accepted.",
            "Implement suggestion here");
    }

    IntegerType::IntegerType(Environment *environment, bool is_signed)
        : Type(environment, is_signed ? "int" : "uint"), is_signed_(is_signed)
    {
        create_parameter("_bw", TypeParameterType::Constant, &bw_type_);
    }

    bool IntegerType::is_signed() const { return is_signed_; }

    bool
    IntegerType::assignable(const Core::Value &value,
                            const std::vector<TypeArgument> &arguments) const
    {
        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr = std::get_if<Core::Value>(&arguments[0]))
                if (auto val_ptr = std::get_if<uint64_t>(ptr))
                    bw = *val_ptr;
        }

        // if (auto ptr =
        //         dynamic_cast<IntegerTypeAssignabilityOptions
        //         *>(options.get()))
        //     bw = ptr->bit_width;

        return std::visit(
            [&](const auto &val) -> bool
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, int64_t> ||
                              std::is_same_v<T, uint64_t>)
                {
                    int64_t min =
                        is_signed_ ? -(static_cast<int64_t>(1) << (bw - 1)) : 0;
                    uint64_t max = bw == 64 ? UINT64_MAX
                                            : (static_cast<uint64_t>(1)
                                               << (is_signed_ ? bw - 1 : bw)) -
                                                  1;

                    return static_cast<int64_t>(val) >= min &&
                           (val >= 0 ? (static_cast<uint64_t>(val) <= max)
                                     : (static_cast<int64_t>(val) <=
                                        static_cast<int64_t>(max)));
                }

                else
                    return false;
            },
            value);
    }

    bool IntegerType::assignable_with([[maybe_unused]] const Type &type) const
    {
        return false;
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> IntegerType::make_suggestion(
        AST::Node *node, const std::vector<TypeArgument> &arguments) const
    {
        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr = std::get_if<Core::Value>(&arguments[0]))
                if (auto val_ptr = std::get_if<uint64_t>(ptr))
                    bw = *val_ptr;
        }

        auto min = is_signed_ ? -(static_cast<int64_t>(1) << (bw - 1)) : 0;
        auto max = (static_cast<uint64_t>(1) << (is_signed_ ? bw - 1 : bw)) - 1;

        return std::make_unique<Diagnostic::NoteDiagnostic>(
            std::move(node), Diagnostic::NoteType::Suggestion,
            std::string("Only values within ") + Diagnostic::NOTE_GEN +
                std::to_string(min) + Utils::Styles::Reset + " to " +
                Diagnostic::NOTE_GEN + std::to_string(max) +
                Utils::Styles::Reset + " are accepted.",
            "Implement suggestion here");
    }

} // namespace Core
