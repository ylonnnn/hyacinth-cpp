#include <cmath>
#include <cstdint>
#include <variant>

#include "core/environment/Environment.hpp"
// #include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"
#include "core/type/TypeParameter.hpp"
#include "core/type/TypePool.hpp"
#include "core/type/builtin/bases/NumericBase.hpp"
#include "core/type/builtin/numeric/Integer.hpp"
#include "core/value/Value.hpp"
#include "core/value/ValuePool.hpp"

namespace Core
{
    IntegerInstantiated::IntegerInstantiated(
        BaseType &base, std::vector<GenericArgument> &&arguments)
        : NumericInstantiated(base, std::move(arguments))
    {
    }

    TypeResult IntegerInstantiated::assignable(Value *value) const
    {
        TypeResult result{ResultStatus::Success, nullptr, {}};

        Diagnostic::Diagnostic *diagnostic = nullptr;
        IntegerType::Signal signal = base.assignable(arguments, value, result);

        Core::PositionRange *range = value->range;
        auto str_type = *to_string();

        if (range != nullptr)
            switch (signal)
            {
                case IntegerType::Mismatch:
                    [[fallthrough]];

                // TODO: Implement some sort of difference
                case IntegerType::Underflow:
                    [[fallthrough]];
                case IntegerType::Overflow:
                    diagnostic = result.error(
                        Core::PositionRange(*range),
                        Diagnostic::ErrorType::TypeMismatch,
                        "expected value of type '" + str_type +
                            "', received '" + value->type->to_string() + "'.");
                    break;

                case IntegerType::Assignable:
                    break; // Assignable

                default:
                    return result;
            }

        if (diagnostic != nullptr)
            diagnostic->add_detail(base.make_suggestion(arguments, value));

        return result;
    }

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

    IntegerType::T *
    IntegerType::create_instance(std::vector<GenericArgument> &&arguments)
    {
        return TYPE_POOL.add(std::make_unique<T>(*this, std::move(arguments)));
    }

    IntegerType::Signal
    IntegerType::assignable(const std::vector<GenericArgument> &arguments,
                            Value *value, TypeResult &result) const
    {
        size_t bw = 32;

        if (!arguments.empty())
        {
            if (auto ptr = std::get_if<Value *>(&arguments[0]))
                if (auto val_ptr = std::get_if<integer>((*ptr)->value.get()))
                    bw = val_ptr->as<uint64_t>();
        }

        auto ptr = std::get_if<integer>(value->value.get());
        if (ptr == nullptr)
            return Mismatch;

        integer &val = *ptr;
        int64_t min = is_signed ? -(1ll << (bw - 1)) : 0;
        uint64_t max = bw == 64
                           ? UINT64_MAX
                           : (1ull << (is_signed ? bw - 1 : bw)) - is_signed;

        if (is_signed)
        {
            auto i64 = val.as<int64_t>();
            return i64 < min                         ? Underflow
                   : i64 > static_cast<int64_t>(max) ? Overflow
                                                     : Assignable;
        }

        else
        {
            if (val.is_negative)
                return Underflow;

            auto u64 = val.as<uint64_t>();
            return u64 < static_cast<uint64_t>(min) ? Underflow
                   : u64 > max                      ? Overflow
                                                    : Assignable;
        }

        return Assignable;
    }

    IntegerType::T *IntegerType::infer(Value &value)
    {
        auto ptr = std::get_if<Core::integer>(value.value.get());
        if (ptr == nullptr)
            return nullptr;

        uint64_t bw = bit_width_of(value);

        return create_instance({VALUE_POOL.add(std::make_unique<Value>(
            std::make_unique<Value::T>(Core::integer(bw, false)),
            bw_type_.create_instance({})))});
    }

    std::unique_ptr<Diagnostic::Diagnostic>
    IntegerType::make_suggestion(const std::vector<GenericArgument> &arguments,
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

        int64_t min = is_signed ? -(1ll << (bw - 1)) : 0;
        uint64_t max = bw == 64
                           ? UINT64_MAX
                           : (1ull << (is_signed ? bw - 1 : bw)) - is_signed;

        return std::make_unique<Diagnostic::Diagnostic>(
            Diagnostic::DiagnosticSeverity::Note,
            static_cast<uint32_t>(Diagnostic::NoteType::Suggestion),
            Core::PositionRange(*value->range),
            "expects values within " + std::to_string(min) + " to " +
                std::to_string(max) + ".");
    }

    uint32_t IntegerType::bit_width_of(Value &value) const
    {
        uint32_t bw = 32;
        auto ptr = std::get_if<Core::integer>(value.value.get());

        if (ptr == nullptr)
            return bw;

        Core::integer &val = *ptr;
        bw = 0;

        auto in_range = [&](uint8_t bw) -> bool
        {
            int64_t min = is_signed ? -(1ll << (bw - 1)) : 0;
            uint64_t max =
                bw == 64 ? UINT64_MAX
                         : (1ull << (is_signed ? bw - 1 : bw)) - is_signed;

            if (is_signed)
            {
                auto i64 = val.as<int64_t>();
                return i64 >= min && i64 <= static_cast<int64_t>(max);
            }

            else
            {
                if (val.is_negative)
                    return Underflow;

                auto u64 = val.as<uint64_t>();
                return u64 >= static_cast<uint64_t>(min) && u64 <= max;
            }
        };

        while (!in_range(++bw))
            ;

        return bw;
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
