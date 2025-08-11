#include <cmath>
#include <cstdint>
#include <type_traits>
#include <variant>

#include "core/environment/Environment.hpp"
#include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"
#include "core/type/primitive/Integer.hpp"
#include "core/value/Value.hpp"
#include "diagnostic/NoteDiagnostic.hpp"

namespace Core
{
    BitWidthType::BitWidthType()
        : BaseType(nullptr, "BitWidth"), min_(1), max_(64)
    {
    }

    bool BitWidthType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        auto ptr = std::get_if<Core::h_int>(&value);
        if (ptr == nullptr)
            return false;

        Core::h_int val = *ptr;
        if (val.i64() < 1)
            return false;

        uint8_t n_val = static_cast<uint8_t>(val.u64());

        return n_val > 0 && n_val <= 64;
    }

    bool
    BitWidthType::assignable_with([[maybe_unused]] const BaseType &type) const
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
                std::to_string(min_) + Utils::Styles::Reset + " to " +
                Diagnostic::NOTE_GEN + std::to_string(max_) +
                Utils::Styles::Reset + " are accepted.",
            "Implement suggestion here");
    }

    bool IntegerType::Wrapper::assignable_with(const Type &type) const
    {
        if (arguments.empty() || type.arguments.empty())
            return this->type->assignable_with(*type.type);

        const TypeArgument &bw = type.arguments[0], &bw_ = arguments[0];
        if (bw.valueless_by_exception() || bw_.valueless_by_exception())
            return false;

        return Type::assignable_with(type) &&
               std::visit(
                   [&](auto &val, auto &val_) -> bool
                   {
                       using Ty = std::decay_t<decltype(val)>;
                       using Ty_ = std::decay_t<decltype(val_)>;

                       if constexpr (std::is_same_v<Ty, Core::Value> &&
                                     std::is_same_v<Ty_, Core::Value>)

                           return std::visit(
                               [&](auto &v, auto &v_) -> bool
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

    IntegerType::IntegerType(Environment *environment, bool is_signed)
        : NumericBase(environment, is_signed ? "int" : "uint"),
          is_signed_(is_signed), int_w_(dynamic_cast<Wrapper *>(
                                     Type::get_or_create<Wrapper>(this, {}))),
          bool_w_(Type::get_or_create(environment_->resolve_type("bool"), {}))
    {
        builtin_ = true;

        add_parameter(TypeParameter{"_Bw", TypeParameterType::Constant,
                                    Type::get_or_create(&bw_type_, {})});

        default_operations();
    }

    IntegerType::~IntegerType() {}

    void IntegerType::default_operations()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        // Arithmetic
        auto __h_bw = [&](Type *left, Type *right) -> Type *
        { return higher_bit_width(left, right); };

        Operation::overload_binary(
            {Operation::BinarySignature{ArithmeticUnary::Plus, int_w_, int_w_},
             true,
             overload<h_int, h_int>([](const h_int &a, const h_int &b) -> h_int
                                    { return a.i64() + b.i64(); }, __h_bw)

            });

        Operation::overload_binary(
            {Operation::BinarySignature{ArithmeticUnary::Minus, int_w_, int_w_},
             true,
             overload<h_int, h_int>([](const h_int &a, const h_int &b) -> h_int
                                    { return a.i64() - b.i64(); }, __h_bw)});

        Operation::overload_binary(
            {Operation::BinarySignature{Arithmetic::Multiplication, int_w_,
                                        int_w_},
             true,
             overload<h_int, h_int>([](const h_int &a, const h_int &b) -> h_int
                                    { return a.i64() * b.i64(); }, __h_bw)});

        Operation::overload_binary(
            {Operation::BinarySignature{Arithmetic::Division, int_w_, int_w_},
             true,
             overload<h_int, h_int>([](const h_int &a, const h_int &b) -> h_int
                                    { return a.i64() / b.i64(); }, __h_bw)});

        Operation::overload_binary(
            {Operation::BinarySignature{Arithmetic::Modulo, int_w_, int_w_},
             true,
             overload<h_int, h_int>([](const h_int &a, const h_int &b) -> h_int
                                    { return a.i64() % b.i64(); }, __h_bw)});

        // Relational
        auto __rel = [&](Type *, Type *) -> Type * { return bool_w_; };

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::Equal, int_w_, int_w_},
             true,
             overload<h_int, h_int>([](const h_int &a, const h_int &b) -> bool
                                    { return a.i64() == b.i64(); }, __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::NotEqual, int_w_, int_w_},
             true,
             overload<h_int, h_int>([](const h_int &a, const h_int &b) -> bool
                                    { return a.i64() != b.i64(); }, __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::LessThan, int_w_, int_w_},
             true,
             overload<h_int, h_int>(
                 [](const h_int &a, const h_int &b) -> bool
                 {
                     return a.is_signed() ? a.i64() < b.i64()
                                          : a.u64() < b.u64();
                 },
                 __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::LessThanEqual, int_w_,
                                        int_w_},
             true,
             overload<h_int, h_int>(
                 [](const h_int &a, const h_int &b) -> bool
                 {
                     return a.is_signed() ? a.i64() <= b.i64()
                                          : a.u64() <= b.u64();
                 },
                 __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::GreaterThan, int_w_,
                                        int_w_},
             true,
             overload<h_int, h_int>(
                 [](const h_int &a, const h_int &b) -> bool
                 {
                     return a.is_signed() ? a.i64() > b.i64()
                                          : a.u64() > b.u64();
                 },
                 __rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::GreaterThanEqual, int_w_,
                                        int_w_},
             true,
             overload<h_int, h_int>(
                 [](const h_int &a, const h_int &b) -> bool
                 {
                     return a.is_signed() ? a.i64() >= b.i64()
                                          : a.u64() >= b.u64();
                 },
                 __rel)});
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
                if (auto val_ptr = std::get_if<Core::h_int>(ptr))
                    bw = val_ptr->u64();
        }

        // if (auto ptr =
        //         dynamic_cast<IntegerTypeAssignabilityOptions
        //         *>(options.get()))
        //     bw = ptr->bit_width;

        return std::visit(
            [&](auto &val) -> bool
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, Core::h_int>)
                {
                    int64_t min = is_signed_ ? -(1ll << (bw - 1)) : 0;
                    uint64_t max =
                        bw == 64
                            ? UINT64_MAX
                            : (1ull << (is_signed_ ? bw - 1 : bw)) - is_signed_;

                    return val.i64() >= min &&
                           (val.i64() >= 0
                                ? (val.u64() <= max)
                                : (val.i64() <= static_cast<int64_t>(max)));
                }

                else
                    return false;
            },
            value);
    }

    Type *IntegerType::construct_wrapper() const
    {
        return Type::get_or_create<Wrapper>(const_cast<IntegerType *>(this),
                                            {});
    }

    Type *IntegerType::construct_wrapper(uint8_t bit_width) const
    {
        return Type::get_or_create<Wrapper>(const_cast<IntegerType *>(this),
                                            {bit_width});
    }

    Type *IntegerType::from_value(const Core::Value &value) const
    {
        auto ptr = std::get_if<Core::h_int>(&value);
        if (ptr == nullptr)
            return nullptr;

        Core::h_int val = *ptr;

        uint8_t bw = 0;
        auto in_range = [&](uint8_t bw) -> bool
        {
            auto is_signed = val.is_signed();

            auto min = is_signed ? -(1ll << (bw - 1)) : 0;
            auto max = bw == 64
                           ? UINT64_MAX
                           : (1ull << (is_signed ? bw - 1 : bw)) - is_signed;

            return val.i64() >= min &&
                   (is_signed ? (val.i64() <= static_cast<int64_t>(max))
                              : (val.u64() <= max));
        };

        while (!in_range(++bw))
        {
        }

        return construct_wrapper(bw);
    }

    bool IntegerType::assignable_with(const BaseType &type) const
    {
        return BaseType::assignable_with(type);
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> IntegerType::make_suggestion(
        AST::Node *node, const std::vector<TypeArgument> &arguments) const
    {
        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr = std::get_if<Core::Value>(&arguments[0]))
                if (auto val_ptr = std::get_if<Core::h_int>(ptr))
                    if (!val_ptr->is_signed())
                        bw = val_ptr->u64();
        }

        auto min = is_signed_ ? -(1ll << (bw - 1)) : 0;
        auto max = bw == 64 ? UINT64_MAX
                            : (1ull << (is_signed_ ? bw - 1 : bw)) - is_signed_;

        return std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            std::string("Only values within ") + Diagnostic::NOTE_GEN +
                std::to_string(min) + Utils::Styles::Reset + " to " +
                Diagnostic::NOTE_GEN + std::to_string(max) +
                Utils::Styles::Reset + " are accepted.",
            "Implement suggestion here");
    }
} // namespace Core
