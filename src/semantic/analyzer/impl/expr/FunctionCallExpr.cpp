#include <algorithm>

#include "core/symbol/FunctionSymbol.hpp"
#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::FunctionCalLExpr>::analyze(Analyzer &analyzer,
                                                 AST::FunctionCalLExpr &node)
    {
        Core::Environment *current_env = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        // TODO: Analyze Callee
        AnalysisResult c_res =
            AnalyzerImpl<AST::Expr>::analyze(analyzer, node.callee());

        Core::Type *callee_type = c_res.data;

        if (c_res.status == Core::ResultStatus::Fail)
            result.status = c_res.status;

        result.diagnostics.insert(
            result.diagnostics.end(),
            std::make_move_iterator(c_res.diagnostics.begin()),
            std::make_move_iterator(c_res.diagnostics.end()));

        auto &callee = node.callee();

        if (c_res.symbol == nullptr)
        {
            result.error(&callee,
                         Diagnostic::ErrorTypes::Semantic::UnrecognizedSymbol,
                         std::string("Unrecognized symbol detected."),
                         "Unrecognized callee symbol detected here");

            return result;
        }

        auto symbol = dynamic_cast<Core::FunctionSymbol *>(c_res.symbol);
        if (symbol == nullptr)
        {
            result.error(
                &callee,
                Diagnostic::ErrorTypes::Semantic::NonCallableInvocation,
                std::string("Cannot invoke non-callable symbol \"") +
                    Diagnostic::ERR_GEN + c_res.symbol->name +
                    Utils::Styles::Reset + "\".",
                "Invoked here");

            return result;
        }

        result.data = symbol->return_type;

        // TODO: Analyze Arguments

        auto &parameters = symbol->parameters;
        auto &arguments = node.arguments();

        size_t param_count = parameters.size(), arg_count = arguments.size();
        if (param_count != arg_count)
        {
            auto last_idx = param_count == 0 ? 0 : param_count;

            // Too few arguments
            if (last_idx > arg_count)
            {
                last_idx = arg_count - 1;
                result.error(
                    arguments[last_idx].get(),
                    Diagnostic::ErrorTypes::Semantic::InvalidArgumentCount,
                    std::string("Too few arguments provided."),
                    "Arguments provided here");
            }

            // Too much arguments
            else
                result.error(
                    arguments[last_idx].get(),
                    Diagnostic::ErrorTypes::Semantic::InvalidArgumentCount,
                    std::string("Too much arguments provided."),
                    "Arguments provided here");

            return result;
        }

        for (size_t i = 0; i < param_count; i++)
        {
            Core::FunctionParameter parameter = parameters[i];
            std::unique_ptr<AST::Expr> &argument = arguments[i];

            Core::TypeResolutionResult tr_res =
                parameter.resolved_type->resolve(*parameter.type);

            if (tr_res.status == Core::ResultStatus::Fail)
                result.status = tr_res.status;

            result.diagnostics.insert(
                result.diagnostics.end(),
                std::make_move_iterator(tr_res.diagnostics.begin()),
                std::make_move_iterator(tr_res.diagnostics.end()));

            AnalysisResult a_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, *argument);

            if (a_res.status == Core::ResultStatus::Fail)
                result.status = a_res.status;

            result.diagnostics.insert(
                result.diagnostics.end(),
                std::make_move_iterator(a_res.diagnostics.begin()),
                std::make_move_iterator(a_res.diagnostics.end()));

            auto assignable = false;

            if (a_res.value)
                assignable = parameter.resolved_type->assignable(
                    *a_res.value, tr_res.arguments);
            else
                assignable =
                    parameter.resolved_type->assignable_with(*tr_res.data);

            if (!assignable)
            {
                auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                    argument.get(),
                    Diagnostic::ErrorTypes::Type::InvalidArgumentType,
                    std::string(
                        "Invalid argument provided. Expected value of type ") +
                        Diagnostic::ERR_GEN + parameter.type->to_string() +
                        Utils::Styles::Reset + ".",
                    std::string(""));

                diagnostic->add_detail(tr_res.data->make_suggestion(
                    argument.get(), tr_res.arguments));

                result.error(std::move(diagnostic));

                continue;
            }
        }

        return result;
    }

} // namespace Semantic
