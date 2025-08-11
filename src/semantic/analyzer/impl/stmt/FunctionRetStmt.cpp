#include "core/environment/FunctionEnvironment.hpp"
#include "core/symbol/FunctionSymbol.hpp"
#include "core/type/primitive/Void.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    static Core::FunctionEnvironment *resolve_fn_env(Core::Environment *current)
    {
        if (current == nullptr)
            return nullptr;

        while (current != nullptr)
        {
            if (typeid(*current) == typeid(Core::FunctionEnvironment))
                return static_cast<Core::FunctionEnvironment *>(current);

            current = current->parent();
        }

        return nullptr;
    }

    static void analyze_value(Analyzer &analyzer, AST::FunctionReturnStmt &node,
                              AnalysisResult &result,
                              Core::FunctionEnvironment *fn_env)
    {
        AST::Expr *ret_val = node.value();
        auto combination =
            0b00; // 0bxy | x - fn return type (0 - Void | 1 - Any) | y - ret
                  // value (0 - nullptr | 1 - Expr)

        // Return Value
        combination |= ret_val != nullptr;

        // Return Type
        auto *fn = fn_env->fn_symbol();

        Core::Type *ret_type = fn->return_type;
        Core::BaseType *b_type = ret_type->type;

        combination |= ((typeid(*b_type) != typeid(Core::Void)) << 1);

        switch (combination)
        {
            // Valid
            case 0b00:
            case 0b11:
            {
                result.data = ret_type;

                if (ret_val == nullptr)
                {
                    result.value = Core::null{};
                    return;
                }

                // std::cout << "ret_val: ";
                // ret_val->print(std::cout, 1);
                // std::cout << "\n";

                AnalysisResult v_res =
                    AnalyzerImpl<AST::Expr>::analyze(analyzer, *ret_val);
                result.adapt(v_res.status, std::move(v_res.diagnostics));

                auto error = [&]() -> void
                {
                    auto diagnostic =
                        std::make_unique<Diagnostic::ErrorDiagnostic>(
                            ret_val, Diagnostic::ErrorTypes::Type::Mismatch,
                            std::string("Expected value of type ") +
                                Diagnostic::ERR_GEN + ret_type->to_string() +
                                Utils::Styles::Reset + ".",
                            std::string(""));

                    diagnostic->add_detail(
                        result.data->make_suggestion(ret_val));

                    result.error(std::move(diagnostic));
                };

                // Analysis of returned value
                if (v_res.value)
                {
                    if (!result.data->assignable(*v_res.value))
                        error();

                    result.value = std::move(*v_res.value);
                }

                // Analysis of returned type
                else
                {
                    if (v_res.data == nullptr)
                    {
                        error();
                        return;
                    }

                    if (!result.data->assignable_with(*v_res.data))
                        error();
                }

                return;
            }

            // Invalid
            case 0b10:
            {
                auto type_display = Diagnostic::ERR_GEN +
                                    ret_type->to_string() +
                                    Utils::Styles::Reset;

                result.error(
                    &node, Diagnostic::ErrorTypes::Type::InvalidReturnType,
                    std::string("Functions with return type \"") +
                        type_display + " must return a value of type " +
                        type_display + ".",
                    "Must return a value");

                return;
            }

            case 0b01:
            {
                result.error(ret_val,
                             Diagnostic::ErrorTypes::Type::InvalidReturnType,
                             std::string("\"") + Diagnostic::ERR_GEN + "void" +
                                 Utils::Styles::Reset +
                                 "\" functions must not return a value.",
                             "Returns a value here");

                return;
            }
        }
    }

    AnalysisResult AnalyzerImpl<AST::FunctionReturnStmt>::analyze(
        Analyzer &analyzer, AST::FunctionReturnStmt &node)
    {
        Core::Environment *current = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        result.diagnostics.reserve(8);

        Core::FunctionEnvironment *fn_env = resolve_fn_env(current);
        if (fn_env == nullptr)
        {
            result.error(
                &node, Diagnostic::ErrorTypes::Semantic::InvalidReturnUsage,
                std::string("\"") + Diagnostic::ERR_GEN + "return" +
                    Utils::Styles::Reset +
                    "\" can only be used inside of functions.",
                "Used \"return\" keyword outside of a function context.");

            return result;
        }

        analyze_value(analyzer, node, result, fn_env);

        return result;
    }

} // namespace Semantic
