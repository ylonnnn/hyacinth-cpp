#include "core/symbol/FunctionSymbol.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    static bool analyze_callee(Analyzer &analyzer, AST::FunctionCallExpr &node,
                               AnalysisResult &result)
    {
        auto &callee = node.callee();

        AnalysisResult c_res =
            AnalyzerImpl<AST::Expr>::analyze(analyzer, callee);

        result.adapt(c_res.status, std::move(c_res.diagnostics));

        if (c_res.symbol == nullptr)
        {
            result.error(&callee,
                         Diagnostic::ErrorTypes::Semantic::UnrecognizedSymbol,
                         "Unrecognized symbol invoked.", "Invoked here");

            return false;
        }

        if (typeid(*c_res.symbol) != typeid(Core::FunctionSymbol))
        {
            result.error(
                &callee,
                Diagnostic::ErrorTypes::Semantic::NonCallableInvocation,
                std::string("Cannot invoke non-callable symbol \"") +
                    Diagnostic::ERR_GEN + std::string(c_res.symbol->name) +
                    utils::Styles::Reset + "\".",
                "Invoked here");

            return false;
        }

        auto symbol = static_cast<Core::FunctionSymbol *>(c_res.symbol);

        result.symbol = symbol;
        result.data = symbol->return_type;

        node.set_fn_symbol(symbol);

        return true;
    }

    static void analyze_arguments(Analyzer &analyzer,
                                  AST::FunctionCallExpr &node,
                                  AnalysisResult &result)
    {
        auto symbol = static_cast<Core::FunctionSymbol *>(result.symbol);

        auto &parameters = symbol->parameters;
        auto &arguments = node.arguments();

        size_t param_count = parameters.size(), arg_count = arguments.size();
        if (param_count != arg_count)
        {
            auto diagnostic = Diagnostic::create_invalid_arguments_error(
                &node, param_count, arg_count);

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                symbol->node, Diagnostic::NoteType::Declaration,
                std::string("Function is declared with a different signature "
                            "from the intended usage."),
                "Declared here"));

            result.error(std::move(diagnostic));

            return;
        }

        if (param_count == 0)
            return;

        for (size_t i = 0; i < param_count; i++)
        {
            Core::FunctionParameter &parameter = parameters[i];
            std::unique_ptr<AST::Expr> &argument = arguments[i];

            AnalysisResult a_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, *argument);
            result.adapt(a_res.status, std::move(a_res.diagnostics));

            auto has_value = a_res.value != nullptr;

            if ((has_value && parameter.type->assignable(*a_res.value)) ||
                (a_res.data != nullptr &&
                 parameter.type->assignable_with(*a_res.data)))
            {
                if (has_value)
                    argument->set_value(a_res.value);

                continue;
            }

            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                argument.get(),
                Diagnostic::ErrorTypes::Type::InvalidArgumentType,
                std::string(
                    "Invalid argument provided. Expected value of type ") +
                    Diagnostic::ERR_GEN + parameter.type->to_string() +
                    utils::Styles::Reset + ".",
                std::string(""));

            diagnostic->add_detail(
                parameter.type->make_suggestion(argument.get()));

            result.error(std::move(diagnostic));

            continue;
        }
    }

    AnalysisResult
    AnalyzerImpl<AST::FunctionCallExpr>::analyze(Analyzer &analyzer,
                                                 AST::FunctionCallExpr &node)
    {
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        if (!analyze_callee(analyzer, node, result))
            return result;

        analyze_arguments(analyzer, node, result);

        return result;
    }

} // namespace Semantic
