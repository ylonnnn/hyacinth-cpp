#include <utility>

#include "analyzer/impl/Expr.hpp"
// #include "core/operation/Operation.hpp"
// #include "core/symbol/FunctionSymbol.hpp"
// #include "core/symbol/LibSymbol.hpp"
// #include "core/type/compound/Array.hpp"
// #include "core/type/primitive/Integer.hpp"
#include "lexer/Token.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::BinaryExpr>::analyze(Analyzer &analyzer,
                                                          AST::BinaryExpr &node)
    {
        AnalysisResult result{
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        utils::todo("implement AST::BinaryExpr analyzer");

        return result;
    }

    // using SpecialOperatorHandler =
    //     std::function<void(Analyzer &, AST::BinaryExpr &, AnalysisResult &)>;
    // static std::unordered_map<Lexer::TokenType, SpecialOperatorHandler>
    //     special_handler;

    // void default_special_handlers()
    // {
    //     using namespace Lexer::TokenTypes;
    //     using namespace Operator;

    //     special_handler.try_emplace(
    //         Access::DoubleColon,
    //         [&](Analyzer &analyzer, AST::BinaryExpr &node,
    //             AnalysisResult &result) -> void
    //         {
    //             auto __an_f = [&](AnalysisResult &result,
    //                               AST::Expr &expr) -> void
    //             {
    //                 Core::Symbol *basis = result.symbol;
    //                 // std::cout << "expr: " << expr << "\n";

    //                 if (basis == nullptr)
    //                 {
    //                     AnalysisResult l_res = analyzer.analyze(expr);
    //                     result.adapt(l_res.status,
    //                     std::move(l_res.diagnostics),
    //                                  l_res.data);

    //                     result.value = std::move(l_res.value);
    //                     result.symbol = l_res.symbol;

    //                     node.set_value(result.value);

    //                     return;
    //                 }

    //                 if (typeid(*basis) != typeid(Core::LibSymbol))
    //                 {
    //                     result.error(
    //                         basis->node,
    //                         Diagnostic::ErrorTypes::Semantic::UnknownLib,
    //                         std::string("Unknown library \"") +
    //                             Diagnostic::ERR_GEN +
    //                             std::string(basis->name) +
    //                             utils::Styles::Reset + "\" being accessed.",
    //                         "Accessing an unknown library.");

    //                     return;
    //                 }

    //                 auto lib = static_cast<Core::LibSymbol *>(basis);
    //                 auto &t_info = typeid(expr);

    //                 auto expr_error = [&]() -> void
    //                 {
    //                     result.error(
    //                         &expr,
    //                         Diagnostic::ErrorTypes::Syntax::
    //                             UnexpectedExpression,
    //                         std::string("Unexpected expression provided as "
    //                                     "field accessor."),
    //                         "Cannot access fields with this kind of "
    //                         "expression");
    //                 };

    //                 if (t_info == typeid(AST::BinaryExpr))
    //                 {
    //                     auto &b_expr = static_cast<AST::BinaryExpr &>(expr);

    //                     auto s_it =
    //                         special_handler.find(b_expr.operation().type);
    //                     if (s_it == special_handler.end())
    //                     {
    //                         expr_error();
    //                         return;
    //                     }

    //                     Core::Environment *current = analyzer.current_env();
    //                     analyzer.set_current_env(*lib->environment);

    //                     s_it->second(analyzer, b_expr, result);
    //                     analyzer.set_current_env(*current);

    //                     return;
    //                 }

    //                 if (t_info != typeid(AST::IdentifierExpr))
    //                 {
    //                     expr_error();
    //                     return;
    //                 }

    //                 std::string m_name =
    //                     std::string(static_cast<AST::IdentifierExpr &>(expr)
    //                                     .identifier()
    //                                     .value);

    //                 Core::Symbol *resolved =
    //                     lib->environment->resolve_symbol(m_name);

    //                 if (resolved == nullptr)
    //                 {
    //                     result.error(
    //                         &expr,
    //                         Diagnostic::ErrorTypes::Semantic::
    //                             UnrecognizedSymbol,
    //                         std::string("Unrecognized symbol \"") +
    //                             Diagnostic::ERR_GEN + std::string(m_name) +
    //                             utils::Styles::Reset + "\" being accessed.",
    //                         "Accessing an unrecognized symbol from library
    //                         \"" +
    //                             std::string(basis->name) + "\".");

    //                     return;
    //                 }

    //                 if (auto ptr =
    //                         dynamic_cast<Core::IdentifierSymbol *>(resolved))
    //                 {
    //                     result.data = ptr->type;
    //                     result.value = ptr->value;
    //                 }

    //                 result.symbol = resolved;
    //             };

    //             __an_f(result, node.left());
    //             if (result.status == Core::ResultStatus::Fail)
    //                 return;

    //             __an_f(result, node.right());
    //         });

    //     special_handler.try_emplace(
    //         Access::Dot,
    //         [&](Analyzer &analyzer, AST::BinaryExpr &node,
    //             AnalysisResult &result) -> void
    //         {
    //             auto __an_f = [&](AnalysisResult &result,
    //                               AST::Expr &expr) -> void
    //             {
    //                 Core::Type *basis = result.data;
    //                 std::shared_ptr<Core::Value> &b_val = result.value;

    //                 if (basis == nullptr)
    //                 {
    //                     AnalysisResult l_res = analyzer.analyze(expr);
    //                     result.adapt(l_res.status,
    //                     std::move(l_res.diagnostics),
    //                                  l_res.data);

    //                     result.value = std::move(l_res.value);
    //                     result.symbol = l_res.symbol;

    //                     node.set_value(result.value);

    //                     return;
    //                 }

    //                 auto &t_info = typeid(expr);

    //                 if (t_info == typeid(AST::BinaryExpr))
    //                 {
    //                     auto s_it = special_handler.find(Access::Dot);
    //                     if (s_it == special_handler.end())
    //                         return;

    //                     s_it->second(analyzer,
    //                                  static_cast<AST::BinaryExpr &>(expr),
    //                                  result);

    //                     return;
    //                 }

    //                 if (t_info != typeid(AST::IdentifierExpr))
    //                 {
    //                     AnalyzerImpl<AST::Expr>::analyze(analyzer, expr);
    //                     return;
    //                 }

    //                 std::string m_name =
    //                     std::string(static_cast<AST::IdentifierExpr &>(expr)
    //                                     .identifier()
    //                                     .value);

    //                 auto &members = basis->type->members();
    //                 auto m_it = members.find(m_name);

    //                 if (m_it == members.end())
    //                 {
    //                     result.error(
    //                         &expr,
    //                         Diagnostic::ErrorTypes::Semantic::UnrecognizedField,
    //                         std::string("Unrecognized field \"") +
    //                             Diagnostic::ERR_GEN + m_name +
    //                             utils::Styles::Reset + "\" of type \"" +
    //                             Diagnostic::ERR_GEN + basis->to_string() +
    //                             utils::Styles::Reset + "\".",
    //                         "Unrecognized field provided");

    //                     return;
    //                 }

    //                 auto &member = m_it->second;
    //                 if (auto type = member.as<Core::Type>())
    //                 {
    //                     result.data = type;
    //                     if (!b_val)
    //                         return;

    //                     auto val = b_val ? std::get_if<Core::object>(&*b_val)
    //                                      : nullptr;

    //                     if (val == nullptr)
    //                         return;

    //                     Core::value_data *entry = val->get(m_name);
    //                     if (entry == nullptr)
    //                     {
    //                         // TODO: No entry error handler
    //                         // result.error()
    //                         return;
    //                     }

    //                     result.data = entry->type;
    //                     result.value = entry->value;

    //                     node.set_value(result.value);
    //                 }

    //                 else
    //                 {
    //                     auto fn = member.as<Core::FunctionSymbol>();

    //                     result.data = fn->return_type;
    //                     result.symbol = fn;
    //                 }
    //             };

    //             __an_f(result, node.left());
    //             if (result.status == Core::ResultStatus::Fail)
    //                 return;

    //             __an_f(result, node.right());
    //         });

    //     special_handler.try_emplace(
    //         Delimeter::BracketOpen,
    //         [&](Analyzer &analyzer, AST::BinaryExpr &node,
    //             AnalysisResult &result) -> void
    //         {
    //             AST::Expr &left = node.left(), &right = node.right();

    //             AnalysisResult al_res =
    //                 AnalyzerImpl<AST::Expr>::analyze(analyzer, left);
    //             result.adapt(al_res.status, std::move(al_res.diagnostics));

    //             Core::Type *type = al_res.data;
    //             if (type == nullptr)
    //                 return;

    //             if (typeid(*type) != typeid(Core::ArrayType::Wrapper))
    //             {
    //                 result.error(
    //                     &left,
    //                     Diagnostic::ErrorTypes::Semantic::
    //                         IllegalNonArrayElementAccess,
    //                     std::string(
    //                         "Illegal element access on non-array type \"") +
    //                         Diagnostic::ERR_GEN + type->to_string() +
    //                         utils::Styles::Reset + "\".",
    //                     "Value must be an array type to perform an element "
    //                     "access");

    //                 return;
    //             }

    //             auto *type_w = static_cast<Core::ArrayType::Wrapper *>(type);
    //             result.data = type_w->element_type();

    //             AnalysisResult ar_res =
    //                 AnalyzerImpl<AST::Expr>::analyze(analyzer, right);
    //             result.adapt(ar_res.status, std::move(ar_res.diagnostics));

    //             Core::Type *idx_type = ar_res.data;
    //             if (idx_type == nullptr)
    //                 return;

    //             Core::BaseType *idx_btype = idx_type->type;

    //             if (typeid(*idx_btype) != typeid(Core::IntegerType))
    //             {
    //                 result.error(&right,
    //                 Diagnostic::ErrorTypes::Type::Mismatch,
    //                              "Array indices used for element access must
    //                              " "be an integer.", std::string("Index is of
    //                              type \"") +
    //                                  idx_type->to_string() + "\".");

    //                 return;
    //             }

    //             if (al_res.value != nullptr && ar_res.value != nullptr)
    //             {
    //                 auto &val = std::get<Core::array>(*al_res.value);
    //                 auto &idx = std::get<Core::h_int>(*ar_res.value);

    //                 int64_t index = idx.i64();
    //                 size_t n = val.size();

    //                 if (index < 0 || static_cast<size_t>(index) >= n)
    //                 {
    //                     size_t max = n == 0 ? 0 : n - 1;

    //                     result.error(
    //                         &right,
    //                         Diagnostic::ErrorTypes::Semantic::OutOfRange,
    //                         std::string("Invalid index \"") +
    //                             Diagnostic::ERR_GEN + std::to_string(index) +
    //                             utils::Styles::Reset + "\" provided.",
    //                         max ? (std::string("Valid index range 0-") +
    //                                std::to_string(max) + ".")
    //                             : "Cannot access an element of an empty
    //                             array");

    //                     return;
    //                 }

    //                 Core::value_data *data = val.get(index);

    //                 result.data = data->type;
    //                 result.value = data->value;

    //                 node.set_value(result.value);
    //             }

    //             return;
    //         });
    // }

    // bool initialize()
    // {
    //     using namespace Lexer::TokenTypes;
    //     using namespace Operator;

    //     special_handler.reserve(8);

    //     default_special_handlers();

    //     return true;
    // }

    // bool initialized = initialize();

    // AnalysisResult AnalyzerImpl<AST::BinaryExpr>::analyze(Analyzer &analyzer,
    //                                                       AST::BinaryExpr
    //                                                       &node)
    // {
    //     using namespace Lexer::TokenTypes;
    //     using namespace Operator;

    //     AnalysisResult result = {
    //         nullptr, Core::ResultStatus::Success, nullptr, {}};

    //     auto op = node.operation().type;
    //     auto s_it = special_handler.find(op);
    //     if (s_it != special_handler.end())
    //     {
    //         s_it->second(analyzer, node, result);
    //         return result;
    //     }

    //     AnalysisResult l_res =
    //         AnalyzerImpl<AST::Expr>::analyze(analyzer, node.left());

    //     result.adapt(l_res.status, std::move(l_res.diagnostics));
    //     if (result.status == Core::ResultStatus::Fail)
    //     {

    //         return result;
    //     }

    //     AnalysisResult r_res =
    //         AnalyzerImpl<AST::Expr>::analyze(analyzer, node.right());

    //     result.adapt(r_res.status, std::move(r_res.diagnostics));
    //     if (result.status == Core::ResultStatus::Fail)
    //     {

    //         return result;
    //     }

    //     auto [operation, flipped] =
    //         Core::Operation::get_binary_overload({op, l_res.data,
    //         r_res.data});

    //     if (operation == nullptr)
    //     {
    //         result.error(
    //             &node,
    //             Diagnostic::ErrorTypes::Semantic::NoViableOperatorOverload,
    //             std::string("No viable operator overload for ") +
    //                 Diagnostic::ERR_GEN + l_res.data->to_string() + " " +
    //                 std::string(node.operation().value) + " " +
    //                 r_res.data->to_string() + utils::Styles::Reset + ".",
    //             "");

    //         return result;
    //     }

    //     if (flipped)
    //         std::swap(l_res, r_res);

    //     std::optional<Core::Operation::Assignee> res_val =
    //     operation->handler(
    //         Core::Operation::Operand{l_res.data, l_res.value},
    //         Core::Operation::Operand{r_res.data, r_res.value});

    //     if (!res_val)
    //     {
    //         // result.error();
    //         return result;
    //     }

    //     result.data = res_val->type;
    //     result.value = res_val->value;

    //     node.set_value(result.value);

    //     std::cout << "type: "
    //               << (result.data != nullptr ? result.data->to_string()
    //                                          : "none")
    //               << "\n";

    //     std::cout << "value: " << (result.value ? *result.value : "none")
    //               << "\n";

    //     return result;
    // }

} // namespace Semantic
