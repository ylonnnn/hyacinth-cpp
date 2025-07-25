#include <algorithm>

#include "core/symbol/FunctionSymbol.hpp"
#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    bool analyze_callee(Analyzer &analyzer, AST::FunctionCalLExpr &node,
                        AnalysisResult &result)
    {
        auto &callee = node.callee();

        AnalysisResult c_res =
            AnalyzerImpl<AST::Expr>::analyze(analyzer, callee);

        Core::Type *callee_type = c_res.data;

        result.adapt(c_res.status, std::move(c_res.diagnostics));

        if (c_res.symbol == nullptr)
        {
            result.error(&callee,
                         Diagnostic::ErrorTypes::Semantic::UnrecognizedSymbol,
                         std::string("Unrecognized symbol detected."),
                         "Unrecognized callee symbol detected here");

            return false;
        }

        if (typeid(*c_res.symbol) != typeid(Core::FunctionSymbol))
        {
            result.error(
                &callee,
                Diagnostic::ErrorTypes::Semantic::NonCallableInvocation,
                std::string("Cannot invoke non-callable symbol \"") +
                    Diagnostic::ERR_GEN + c_res.symbol->name +
                    Utils::Styles::Reset + "\".",
                "Invoked here");

            return false;
        }

        auto symbol = static_cast<Core::FunctionSymbol *>(c_res.symbol);

        result.symbol = symbol;
        result.data = symbol->return_type.get();

        return true;
    }

    AnalysisResult
    AnalyzerImpl<AST::FunctionCalLExpr>::analyze(Analyzer &analyzer,
                                                 AST::FunctionCalLExpr &node)
    {
        Core::Environment *current_env = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        if (!analyze_callee(analyzer, node, result))
            return result;

        // TODO: Analyze Arguments

        auto symbol = static_cast<Core::FunctionSymbol *>(result.symbol);

        auto &parameters = symbol->parameters;
        auto &arguments = node.arguments();

        size_t param_count = parameters.size(), arg_count = arguments.size();
        if (param_count != arg_count)
        {
            auto arg__ = [](size_t n) -> std::string
            { return std::string("argument") + (n > 1 ? "s" : ""); };

            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &node, Diagnostic::ErrorTypes::Semantic::InvalidArgumentCount,
                std::string("Function expects ") + Diagnostic::ERR_GEN +
                    std::to_string(param_count) + Utils::Styles::Reset + " " +
                    arg__(param_count) + ". Provided " + Diagnostic::ERR_GEN +
                    std::to_string(arg_count) + Utils::Styles::Reset + " " +
                    arg__(arg_count) + ".",
                "");

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                symbol->node, Diagnostic::NoteType::Declaration,
                std::string("Function is declared with a different signature "
                            "from the intended usage."),
                "Declared here"));

            result.error(std::move(diagnostic));

            return result;
        }

        for (size_t i = 0; i < param_count; i++)
        {
            Core::FunctionParameter &parameter = parameters[i];
            std::unique_ptr<AST::Expr> &argument = arguments[i];

            AnalysisResult a_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, *argument);

            result.adapt(a_res.status, std::move(a_res.diagnostics));

            if (a_res.value ? parameter.type->assignable(*a_res.value)
                            : parameter.type->assignable_with(*a_res.data))
                continue;

            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                argument.get(),
                Diagnostic::ErrorTypes::Type::InvalidArgumentType,
                std::string(
                    "Invalid argument provided. Expected value of type ") +
                    Diagnostic::ERR_GEN + parameter.type->to_string() +
                    Utils::Styles::Reset + ".",
                std::string(""));

            diagnostic->add_detail(
                parameter.type->make_suggestion(argument.get()));

            result.error(std::move(diagnostic));

            continue;
        }

        return result;
    }

} // namespace Semantic
