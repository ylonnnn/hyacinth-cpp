#include <cmath>
#include <cstdint>
#include <variant>

#include "core/environment/Environment.hpp"
// #include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"
#include "core/type/TypeParameter.hpp"
#include "core/type/builtin/bases/NumericBase.hpp"
#include "core/type/builtin/numeric/Integer.hpp"
#include "core/value/Value.hpp"
#include "utils/dev.hpp"

namespace Core
{
    // bool IntegerType::Wrapper::assignable_with(const Type &type) const
    // {
    //     if (arguments.empty() || type.arguments.empty())
    //         return this->type->assignable_with(*type.type);

    //     const GenericArgument &bw = type.arguments[0], &bw_ = arguments[0];
    //     if (bw.valueless_by_exception() || bw_.valueless_by_exception())
    //         return false;

    //     return Type::assignable_with(type) &&
    //            std::visit(
    //                [&](auto &val, auto &val_) -> bool
    //                {
    //                    using Ty = std::decay_t<decltype(val)>;
    //                    using Ty_ = std::decay_t<decltype(val_)>;

    //                    if constexpr (std::is_same_v<
    //                                      Ty, std::shared_ptr<Core::Value>> &&
    //                                  std::is_same_v<
    //                                      Ty_, std::shared_ptr<Core::Value>>)

    //                        return std::visit(
    //                            [&](auto &v, auto &v_) -> bool
    //                            {
    //                                using T = std::decay_t<decltype(v)>;
    //                                using T_ = std::decay_t<decltype(v_)>;

    //                                if constexpr (std::is_same_v<T,
    //                                                             Core::h_int>
    //                                                             &&
    //                                              std::is_same_v<T_,
    //                                                             Core::h_int>)
    //                                    return v.u64() <= v_.u64();

    //                                else
    //                                    return false;
    //                            },
    //                            *val, *val_);

    //                    else
    //                        return false;
    //                },
    //                bw, bw_);
    // }

    IntegerType::IntegerType(Environment &environment, bool is_signed)
        : NumericBase(environment, is_signed ? "int" : "uint"),
          bw_type_(environment)
    {
        add_parameter(TypeParameterType::Value, "_Bw",
                      bw_type_.create_instance({}));
    }

    // IntegerType::IntegerType(Environment *environment, bool is_signed)
    //     : NumericBase(environment, is_signed ? "int" : "uint"),
    //       is_signed_(is_signed), int_w_(dynamic_cast<Wrapper *>(
    //                                  Type::get_or_create<Wrapper>(this,
    //                                  {}))),
    //       bool_w_(Type::get_or_create(environment_->resolve_type("bool"),
    //       {}))
    // {
    //     builtin_ = true;

    //     add_parameter(TypeParameter{"_Bw", TypeParameterType::Constant,
    //                                 Type::get_or_create(&bw_type_, {})});

    //     default_operations();
    // }

    IntegerType::~IntegerType() {}

    void IntegerType::default_operations()
    {
        // // Arithmetic
        // auto __h_bw = [&](Type *left, Type *right) -> Type *
        // { return higher_bit_width(left, right); };

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Arithmetic::Plus, int_w_, int_w_},
        //     true,
        //      overload<h_int, h_int>([](const h_int &a, const h_int &b) ->
        //      h_int
        //                             { return a.i64() + b.i64(); }, __h_bw)

        //     });

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Arithmetic::Minus, int_w_, int_w_},
        //      true,
        //      overload<h_int, h_int>([](const h_int &a, const h_int &b) ->
        //      h_int
        //                             { return a.i64() - b.i64(); }, __h_bw)});

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Arithmetic::Multiplication, int_w_,
        //                                 int_w_},
        //      true,
        //      overload<h_int, h_int>([](const h_int &a, const h_int &b) ->
        //      h_int
        //                             { return a.i64() * b.i64(); }, __h_bw)});

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Arithmetic::Division, int_w_,
        //     int_w_},
        //      true,
        //      overload<h_int, h_int>([](const h_int &a, const h_int &b) ->
        //      h_int
        //                             { return a.i64() / b.i64(); }, __h_bw)});

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Arithmetic::Modulo, int_w_, int_w_},
        //      true,
        //      overload<h_int, h_int>([](const h_int &a, const h_int &b) ->
        //      h_int
        //                             { return a.i64() % b.i64(); }, __h_bw)});

        // // Relational
        // auto __rel = [&](Type *, Type *) -> Type * { return bool_w_; };

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Relational::Equal, int_w_, int_w_},
        //      true,
        //      overload<h_int, h_int>([](const h_int &a, const h_int &b) ->
        //      bool
        //                             { return a.i64() == b.i64(); }, __rel)});

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Relational::NotEqual, int_w_,
        //     int_w_},
        //      true,
        //      overload<h_int, h_int>([](const h_int &a, const h_int &b) ->
        //      bool
        //                             { return a.i64() != b.i64(); }, __rel)});

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Relational::LessThan, int_w_,
        //     int_w_},
        //      true,
        //      overload<h_int, h_int>(
        //          [](const h_int &a, const h_int &b) -> bool
        //          {
        //              return a.is_signed() ? a.i64() < b.i64()
        //                                   : a.u64() < b.u64();
        //          },
        //          __rel)});

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Relational::LessThanEqual, int_w_,
        //                                 int_w_},
        //      true,
        //      overload<h_int, h_int>(
        //          [](const h_int &a, const h_int &b) -> bool
        //          {
        //              return a.is_signed() ? a.i64() <= b.i64()
        //                                   : a.u64() <= b.u64();
        //          },
        //          __rel)});

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Relational::GreaterThan, int_w_,
        //                                 int_w_},
        //      true,
        //      overload<h_int, h_int>(
        //          [](const h_int &a, const h_int &b) -> bool
        //          {
        //              return a.is_signed() ? a.i64() > b.i64()
        //                                   : a.u64() > b.u64();
        //          },
        //          __rel)});

        // Operation::overload_binary(
        //     {Operation::BinarySignature{Relational::GreaterThanEqual, int_w_,
        //                                 int_w_},
        //      true,
        //      overload<h_int, h_int>(
        //          [](const h_int &a, const h_int &b) -> bool
        //          {
        //              return a.is_signed() ? a.i64() >= b.i64()
        //                                   : a.u64() >= b.u64();
        //          },
        //          __rel)});
    }

    TypeResult
    IntegerType::assignable(const std::vector<GenericArgument> &arguments,
                            Value *value) const
    {
        TypeResult result{ResultStatus::Success, nullptr, {}};
        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr = std::get_if<Value *>(&arguments[0]))
                if (auto val_ptr = std::get_if<integer>((*ptr)->value.get()))
                    bw = val_ptr->as<uint64_t>();
        }

        auto error = [&result, &todo = utils::todo]() -> void
        {
            utils::todo(
                "throw error: expected value of type '{}', received '{}'");
        };

        auto ptr = std::get_if<integer>(value->value.get());
        if (ptr == nullptr)
        {
            error();

            return result;
        }

        auto error_suggest = [&result, &error, &todo = utils::todo]() -> void
        {
            error();
            utils::todo("suggest: values within {min} to {max} are the "
                        "expected values");
        };

        integer &val = *ptr;
        if (val.is_neg && !is_signed)
        {
            error_suggest();
            return result;
        }

        int64_t min = is_signed ? -(1ll << (bw - 1)) : 0;
        uint64_t max = bw == 64
                           ? UINT64_MAX
                           : (1ull << (is_signed ? bw - 1 : bw)) - is_signed;

        if (is_signed)
        {
            auto i64 = val.as<int64_t>();
            if (min < i64 || i64 > static_cast<int64_t>(max))
            {
                error_suggest();
                return result;
            }
        }

        else
        {
            auto u64 = val.as<uint64_t>();
            if (static_cast<uint64_t>(min) < u64 || u64 > max)
            {
                error_suggest();
                return result;
            }
        }

        return result;
    }

    // Type *
    // IntegerType::construct_wrapper(std::vector<GenericArgument> &&arguments)
    // const
    // {
    //     return Type::get_or_create<Wrapper>(const_cast<IntegerType *>(this),
    //                                         std::move(arguments));
    // }

    // Type *IntegerType::construct_wrapper(uint8_t bit_width) const
    // {
    //     return Type::get_or_create<Wrapper>(
    //         const_cast<IntegerType *>(this),
    //         {std::make_shared<Core::Value>(bit_width)});
    // }

    // Type *IntegerType::from_value(const Core::Value &value) const
    // {
    //     auto ptr = std::get_if<Core::h_int>(&value);
    //     if (ptr == nullptr)
    //         return nullptr;

    //     Core::h_int val = *ptr;

    //     uint8_t bw = 0;
    //     auto in_range = [&](uint8_t bw) -> bool
    //     {
    //         auto is_signed = val.is_signed();

    //         auto min = is_signed ? -(1ll << (bw - 1)) : 0;
    //         auto max = bw == 64
    //                        ? UINT64_MAX
    //                        : (1ull << (is_signed ? bw - 1 : bw)) - is_signed;

    //         return val.i64() >= min &&
    //                (is_signed ? (val.i64() <= static_cast<int64_t>(max))
    //                           : (val.u64() <= max));
    //     };

    //     while (!in_range(++bw))
    //     {
    //     }

    //     return construct_wrapper(bw);
    // }

    // bool IntegerType::assignable_with(const BaseType &type) const
    // {
    //     return BaseType::assignable_with(type);
    // }

    // std::unique_ptr<Diagnostic::NoteDiagnostic> IntegerType::make_suggestion(
    //     AST::Node *node, const std::vector<GenericArgument> &arguments) const
    // {
    //     size_t bw = 32;

    //     if (!arguments.empty())
    //     {
    //         if (auto ptr =
    //                 std::get_if<std::shared_ptr<Core::Value>>(&arguments[0]))
    //             if (auto val_ptr = std::get_if<Core::h_int>(&**ptr))
    //                 if (!val_ptr->is_signed())
    //                     bw = val_ptr->u64();
    //     }

    //     auto min = is_signed_ ? -(1ll << (bw - 1)) : 0;
    //     auto max = bw == 64 ? UINT64_MAX
    //                         : (1ull << (is_signed_ ? bw - 1 : bw)) -
    //                         is_signed_;

    //     return std::make_unique<Diagnostic::NoteDiagnostic>(
    //         node, Diagnostic::NoteType::Suggestion,
    //         std::string("Only values within ") + Diagnostic::NOTE_GEN +
    //             std::to_string(min) + utils::Styles::Reset + " to " +
    //             Diagnostic::NOTE_GEN + std::to_string(max) +
    //             utils::Styles::Reset + " are accepted.",
    //         "Implement suggestion here");
    // }
} // namespace Core
