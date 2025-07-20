#include "core/symbol/FunctionSymbol.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::FunctionDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::FunctionDeclarationStmt &node)
    {
        Core::Environment &current = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        auto function =
            std::make_unique<Core::FunctionSymbol>(Core::FunctionSymbol{
                {std::string(node.name().value), node.position()},
                current.resolve_type(
                    std::string(node.return_type().value().value)),
                {}});

        for (auto &param : node.parameters())
        {
            Core::Type *type =
                current.resolve_type(std::string(param->type().value().value));
            if (type == nullptr)
            {
                result.error(&param->type(),
                             Diagnostic::ErrorTypes::Type::UnknownType,
                             std::string(""), std::string(""));

                continue;
            }

            result.data = type;
            Core::FunctionParameter parameter{param->name().value,
                                              param->is_mutable(), type};

            if (node.is_definition())
            {
                // TODO: Register Symbols
            }

            function->parameters.push_back(std::move(parameter));
        }

        // auto variable =
        //     std::make_unique<Core::VariableSymbol>(Core::VariableSymbol{
        //         {std::string(node.name().value), node.position()},
        //         node.is_mutable(),
        //         current.resolve_type(std::string(node.type().value().value)),
        //         Core::null{}});

        // Core::Type *type = variable->type;
        // result.data = type;

        // Core::TypeResolutionResult t_res = type->resolve(node.type());

        // if (t_res.status == Core::ResultStatus::Fail)
        //     result.status = t_res.status;

        // result.diagnostics.insert(
        //     result.diagnostics.end(),
        //     std::make_move_iterator(t_res.diagnostics.begin()),
        //     std::make_move_iterator(t_res.diagnostics.end()));

        // if (node.is_definition())
        // {
        //     auto &stmt = static_cast<AST::VariableDefinitionStmt &>(node);
        //     AST::Expr &value = stmt.value();
        //     AnalysisResult v_res = analyzer.analyze(stmt.value());

        //     // Analysis returned a value to be evaluated
        //     if (v_res.value)
        //         if (!t_res.data->assignable(*v_res.value, t_res.arguments))
        //         {
        //             auto diagnostic =
        //                 std::make_unique<Diagnostic::ErrorDiagnostic>(
        //                     &value, Diagnostic::ErrorTypes::Type::Mismatch,
        //                     std::string("Expected value of type ") +
        //                         Diagnostic::ERR_GEN + node.type().to_string()
        //                         + Utils::Styles::Reset + ".",
        //                     std::string(""));

        //             diagnostic->add_detail(
        //                 t_res.data->make_suggestion(&value,
        //                 t_res.arguments));

        //             result.error(std::move(diagnostic));

        //             // result.error(std::move(stmt.value_ptr()),
        //             //              Diagnostic::ErrorTypes::Type::Mismatch,
        //             //              std::string("Expected value of type ") +
        //             //                  Diagnostic::ERR_GEN +
        //             //                  node.type().to_string() +
        //             //                  Utils::Styles::Reset + ".",
        //             //              std::string(""));

        //             // result.note(t_res.data->make_suggestion(,
        //             // t_res.arguments));
        //         }

        //     if (v_res.status == Core::ResultStatus::Fail)
        //         result.status = v_res.status;

        //     result.diagnostics.insert(
        //         result.diagnostics.end(),
        //         std::make_move_iterator(v_res.diagnostics.begin()),
        //         std::make_move_iterator(v_res.diagnostics.end()));
        // }

        return result;
    }

} // namespace Semantic
