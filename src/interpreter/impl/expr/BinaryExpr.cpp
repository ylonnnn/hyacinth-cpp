#include <utility>

#include "core/environment/Environment.hpp"
#include "core/operation/Operation.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Expr.hpp"
#include "lexer/Token.hpp"

namespace Interpreter
{
    using SpecialOperatorHandler = std::function<void(
        Interpreter &, AST::BinaryExpr &, InterpretationResult &)>;
    static std::unordered_map<Lexer::TokenType, SpecialOperatorHandler>
        special_handler;

    void default_special_handlers()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        special_handler.try_emplace(
            Access::Dot,
            [&](Interpreter &interpreter, AST::BinaryExpr &node,
                InterpretationResult &result) -> void
            {
                auto __an_f = [&](InterpretationResult &result,
                                  AST::Expr &expr) -> void
                {
                    if (result.data == nullptr)
                        result.data = expr.value_ptr();

                    std::shared_ptr<Core::Value> &value = result.data;

                    if (value == nullptr)
                    {
                        InterpretationResult l_res =
                            InterpreterImpl<AST::Expr>::interpret(interpreter,
                                                                  expr);
                        result.adapt(l_res.status, std::move(l_res.diagnostics),
                                     l_res.data);

                        result.symbol = l_res.symbol;
                        node.set_value(result.data);

                        return;
                    }

                    auto &t_info = typeid(expr);

                    if (t_info == typeid(AST::BinaryExpr))
                    {
                        auto s_it = special_handler.find(Access::Dot);
                        if (s_it == special_handler.end())
                            return;

                        s_it->second(interpreter,
                                     static_cast<AST::BinaryExpr &>(expr),
                                     result);

                        return;
                    }

                    if (t_info != typeid(AST::IdentifierExpr))
                    {
                        InterpreterImpl<AST::Expr>::interpret(interpreter,
                                                              expr);
                        return;
                    }

                    std::string m_name =
                        std::string(static_cast<AST::IdentifierExpr &>(expr)
                                        .identifier()
                                        .value);

                    auto &obj = std::get<Core::object>(*value);

                    Core::value_data *entry = obj.get(m_name);

                    // Assuming that entry is not a nullptr
                    // If it was, it would have been caught by the analyzer

                    result.data = entry->value;

                    // TODO: Object Methods

                    // auto &member = m_it->second;
                    // if (auto type = member.as<Core::Type>())
                    // {
                    //     result.data = type;
                    //     if (!b_val)
                    //         return;

                    //     auto val = b_val ? std::get_if<Core::object>(&*b_val)
                    //                      : nullptr;

                    //     if (val == nullptr)
                    //         return;

                    //     Core::value_data *entry = val->get(m_name);
                    //     if (entry == nullptr)
                    //     {
                    //         // TODO: No entry error handler
                    //         // result.error()
                    //         return;
                    //     }

                    //     result.data = entry->type;
                    //     result.value = entry->value;

                    //     node.set_value(result.value);
                    // }

                    // else
                    // {
                    //     auto fn = member.as<Core::FunctionSymbol>();

                    //     result.data = fn->return_type;
                    //     result.symbol = fn;
                    // }
                };

                if (result.data != nullptr)
                    return;
                else
                {
                    result.data = node.value_ptr();
                    if (result.data != nullptr)
                        return;
                }

                __an_f(result, node.left());
                if (result.status == Core::ResultStatus::Fail)
                    return;

                __an_f(result, node.right());
            });

        special_handler.try_emplace(
            Delimeter::BracketOpen,
            [&](Interpreter &interpreter, AST::BinaryExpr &node,
                InterpretationResult &result) -> void
            {
                AST::Expr &left = node.left(), &right = node.right();

                InterpretationResult al_res =
                    InterpreterImpl<AST::Expr>::interpret(interpreter, left);
                result.adapt(al_res.status, std::move(al_res.diagnostics));

                std::shared_ptr<Core::Value> &value = al_res.data;
                if (value == nullptr)
                {
                    std::cout << "value is nullptr (@element_access)\n";
                    return;
                }

                // if (typeid(*type) != typeid(Core::ArrayType::Wrapper))
                // {
                //     result.error(
                //         &left,
                //         Diagnostic::ErrorTypes::Semantic::
                //             IllegalNonArrayElementAccess,
                //         std::string(
                //             "Illegal element access on non-array type \"") +
                //             Diagnostic::ERR_GEN + type->to_string() +
                //             Utils::Styles::Reset + "\".",
                //         "Value must be an array type to perform an element "
                //         "access");

                //     return;
                // }

                auto &arr = std::get<Core::array>(*value);

                // auto *type_w = static_cast<Core::ArrayType::Wrapper *>(type);
                // result.data = type_w->element_type();

                InterpretationResult ar_res =
                    InterpreterImpl<AST::Expr>::interpret(interpreter, right);
                result.adapt(ar_res.status, std::move(ar_res.diagnostics));

                std::shared_ptr<Core::Value> &idx_val = ar_res.data;
                if (idx_val == nullptr)
                {
                    std::cout << "idx_val is nullptr (@element_access)\n";
                    return;
                }

                int64_t index = std::get<Core::h_int>(*idx_val).i64();
                size_t n = arr.size();

                if (index < 0 || static_cast<size_t>(index) >= n)
                {
                    size_t max = n == 0 ? 0 : n - 1;

                    result.error(
                        &right, Diagnostic::ErrorTypes::Runtime::OutOfRange,
                        std::string("Invalid index \"") + Diagnostic::ERR_GEN +
                            std::to_string(index) + Utils::Styles::Reset +
                            "\" provided.",
                        max ? (std::string("Valid index range 0-") +
                               std::to_string(max) + ".")
                            : "Cannot access an element of an empty array");

                    return;
                }

                Core::value_data *data = arr.get(index);

                result.data = data->value;

                node.set_value(result.data);
            });
    }

    bool initialize()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        special_handler.reserve(8);

        default_special_handlers();

        return true;
    }

    bool initialized = initialize();

    InterpretationResult
    InterpreterImpl<AST::BinaryExpr>::interpret(Interpreter &interpreter,
                                                AST::BinaryExpr &node)
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        InterpretationResult result = {
            Core::ResultStatus::Success, nullptr, {}};

        auto op = node.operation().type;
        auto s_it = special_handler.find(op);
        if (s_it != special_handler.end())
        {
            s_it->second(interpreter, node, result);
            return result;
        }

        InterpretationResult l_res =
            InterpreterImpl<AST::Expr>::interpret(interpreter, node.left());

        result.adapt(l_res.status, std::move(l_res.diagnostics));
        if (result.status == Core::ResultStatus::Fail)
        {

            return result;
        }

        InterpretationResult r_res =
            InterpreterImpl<AST::Expr>::interpret(interpreter, node.right());

        result.adapt(r_res.status, std::move(r_res.diagnostics));
        if (result.status == Core::ResultStatus::Fail)
        {

            return result;
        }

        if (l_res.data == nullptr || r_res.data == nullptr)
            return result;

        Core::Environment *current = interpreter.current_env();
        Core::Type *left = Core::Type::from_value(current, *l_res.data),
                   *right = Core::Type::from_value(current, *r_res.data);

        auto [operation, flipped] =
            Core::Operation::get_binary_overload({op, left, right});

        if (operation == nullptr)
        {
            result.error(
                &node,
                Diagnostic::ErrorTypes::Semantic::NoViableOperatorOverload,
                std::string("No viable operator overload for ") +
                    Diagnostic::ERR_GEN + left->to_string() + " " +
                    std::string(node.operation().value) + " " +
                    right->to_string() + Utils::Styles::Reset + ".",
                "");

            return result;
        }

        if (flipped)
        {
            std::swap(l_res, r_res);
            std::swap(left, right);
        }

        std::optional<Core::Operation::Assignee> res_val =
            operation->handler(Core::Operation::Operand{left, l_res.data},
                               Core::Operation::Operand{right, r_res.data});

        if (!res_val)
        {
            // result.error();
            return result;
        }

        result.data = res_val->value;

        node.set_value(result.data);

        std::cout << "(interpreter) value: "
                  << (result.data ? *result.data : "none") << "\n";

        return result;
    }

} // namespace Interpreter
