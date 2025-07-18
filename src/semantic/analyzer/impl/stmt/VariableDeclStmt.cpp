#include "ast/stmt/VariableDeclStmt.hpp"
#include "core/symbol/VariableSymbol.hpp"
#include "core/value/Value.hpp"
#include "semantic/analyzer/Analyzer.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::VariableDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::VariableDeclarationStmt &node)
    {
        Core::Environment &current = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        auto variable =
            std::make_unique<Core::VariableSymbol>(Core::VariableSymbol{
                {std::string(node.name().value), node.position()},
                node.is_mutable(),
                current.resolve_type(std::string(node.type().value().value)),
                Core::null{}});

        Core::Type *type = variable->type;
        result.data = type;

        Core::TypeResolutionResult t_res = type->resolve(node.type());

        if (t_res.status == Core::ResultStatus::Fail)
            result.status = t_res.status;

        result.diagnostics.insert(
            result.diagnostics.end(),
            std::make_move_iterator(t_res.diagnostics.begin()),
            std::make_move_iterator(t_res.diagnostics.end()));

        if (node.is_definition())
        {
            auto &stmt = static_cast<AST::VariableDefinitionStmt &>(node);
            AST::Expr &value = stmt.value();
            AnalysisResult v_res = analyzer.analyze(stmt.value());

            // Analysis returned a value to be evaluated
            if (v_res.value)
                if (!t_res.data->assignable(*v_res.value, t_res.arguments))
                {
                    auto diagnostic =
                        std::make_unique<Diagnostic::ErrorDiagnostic>(
                            &value, Diagnostic::ErrorTypes::Type::Mismatch,
                            std::string("Expected value of type ") +
                                Diagnostic::ERR_GEN + node.type().to_string() +
                                Utils::Styles::Reset + ".",
                            std::string(""));

                    diagnostic->add_detail(
                        t_res.data->make_suggestion(&value, t_res.arguments));

                    result.error(std::move(diagnostic));

                    // result.error(std::move(stmt.value_ptr()),
                    //              Diagnostic::ErrorTypes::Type::Mismatch,
                    //              std::string("Expected value of type ") +
                    //                  Diagnostic::ERR_GEN +
                    //                  node.type().to_string() +
                    //                  Utils::Styles::Reset + ".",
                    //              std::string(""));

                    // result.note(t_res.data->make_suggestion(,
                    // t_res.arguments));
                }

            if (v_res.status == Core::ResultStatus::Fail)
                result.status = v_res.status;

            result.diagnostics.insert(
                result.diagnostics.end(),
                std::make_move_iterator(v_res.diagnostics.begin()),
                std::make_move_iterator(v_res.diagnostics.end()));
        }

        return result;
    }

} // namespace Semantic
