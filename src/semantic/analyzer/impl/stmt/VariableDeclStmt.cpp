#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "core/symbol/VariableSymbol.hpp"
#include "core/type/primitive/Void.hpp"
#include "core/value/Value.hpp"
#include "semantic/analyzer/Analyzer.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    void validate_duplication(Analyzer &analyzer,
                              std::unique_ptr<Core::VariableSymbol> &var,
                              AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();
        AST::VariableDeclarationStmt *node = var->node;

        auto declared =
            current->resolve_symbol(std::string(node->name().value));
        if (declared == nullptr)
            return;

        auto defined = declared->defined_at != nullptr,
             is_def = node->is_definition();

        auto error = [&](const std::string &message) -> void
        {
            auto err_node = AST::LiteralExpr(node->name());
            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &err_node, Diagnostic::ErrorTypes::Semantic::Duplication,
                std::move(message),
                "Identifier \"" + declared->name + "\" is already used");

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                declared->node,
                defined ? Diagnostic::NoteType::Definition
                        : Diagnostic::NoteType::Declaration,
                std::string("A symbol identified as \"") +
                    Diagnostic::NOTE_GEN + declared->name +
                    Utils::Styles::Reset + "\" is already declared.",
                "Declared here"));

            result.error(std::move(diagnostic));
        };

        if (defined || is_def)
        {
            error(std::string("Cannot re-declare symbol \"") +
                  Diagnostic::ERR_GEN + declared->name + Utils::Styles::Reset +
                  "\".");

            return;
        }
    }

    void validate_type(Analyzer &analyzer,
                       std::unique_ptr<Core::VariableSymbol> &var,
                       AnalysisResult &result)
    {
        AST::Type &ast_type = var->node->type();
        Core::Type *type = var->type;

        if (type == nullptr)
        {
            result.error(Diagnostic::create_unknown_type_error(&ast_type));
            return;
        }

        if (typeid(*type) == typeid(Core::Void))
        {
            result.error(&ast_type,
                         Diagnostic::ErrorTypes::Type::InvalidVariableType,
                         std::string("Type \"") + Diagnostic::ERR_GEN +
                             ast_type.to_string() + Utils::Styles::Reset +
                             "\" can only be used as a function return type.",
                         "Only functions can have return types of this type");

            return;
        }

        result.data = type;
    }

    void validate_value(Analyzer &analyzer,
                        std::unique_ptr<Core::VariableSymbol> &var,
                        AnalysisResult &result)
    {
        if (var->node == nullptr)
            return;

        AST::VariableDeclarationStmt &node = *var->node;
        Core::TypeResolutionResult t_res = result.data->resolve(node.type());

        result.adapt(t_res.status, std::move(t_res.diagnostics));

        if (node.is_definition())
        {
            auto &stmt = static_cast<AST::VariableDefinitionStmt &>(node);
            AST::Expr &value = stmt.value();
            AnalysisResult v_res = analyzer.analyze(stmt.value());

            result.adapt(v_res.status, std::move(v_res.diagnostics));

            auto error = [&]() -> void
            {
                auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                    &value, Diagnostic::ErrorTypes::Type::Mismatch,
                    std::string("Expected value of type ") +
                        Diagnostic::ERR_GEN + node.type().to_string() +
                        Utils::Styles::Reset + ".",
                    std::string(""));

                diagnostic->add_detail(
                    t_res.data->make_suggestion(&value, t_res.arguments));

                result.error(std::move(diagnostic));
            };

            // Analysis of returned value
            if (v_res.value)
            {
                var->define(const_cast<Core::Position *>(&stmt.position()));

                if (!t_res.data->assignable(*v_res.value, t_res.arguments))
                    error();
            }

            // Analysis of returned type
            else if (v_res.data != nullptr &&
                     !t_res.data->assignable_with(*v_res.data))
                error();
        }
    }

    AnalysisResult AnalyzerImpl<AST::VariableDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::VariableDeclarationStmt &node)
    {
        Core::Environment *current = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        auto &ast_type = node.type();

        auto variable = std::make_unique<Core::VariableSymbol>(
            std::string(node.name().value), node.position(), node.is_mutable(),
            current->resolve_type(std::string(ast_type.value().value)),
            Core::null{}, &node);

        validate_duplication(analyzer, variable, result);

        validate_type(analyzer, variable, result);

        validate_value(analyzer, variable, result);

        current->declare_variable(std::move(variable));

        return result;
    }

} // namespace Semantic
