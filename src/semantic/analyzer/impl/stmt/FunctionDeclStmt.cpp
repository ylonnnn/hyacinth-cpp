#include <optional>

#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "core/symbol/FunctionSymbol.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    void validate_parameters(Analyzer &analyzer,
                             std::unique_ptr<Core::FunctionSymbol> &fn,
                             AnalysisResult &result,
                             Core::Environment *body_env)
    {
        Core::Environment *current = analyzer.current_env();
        AST::FunctionDeclarationStmt *node = fn->node;

        auto is_def = node->is_definition();

        for (auto &param : node->parameters())
        {
            Core::Type *type =
                current->resolve_type(std::string(param->type().value().value));

            // Invalid Parameter Type
            if (type == nullptr)
            {
                result.error(
                    Diagnostic::create_unknown_type_error(&param->type()));

                continue;
            }

            Core::FunctionParameter parameter{
                param->name().value, param->is_mutable(), &param->type(), type};

            // Register/Declare the parameters as variables to the environment
            // for analysis
            if (is_def)
            {
                auto p_variable = std::make_unique<Core::VariableSymbol>(
                    std::string(param->name().value), param->position(),
                    param->is_mutable(), type, Core::Value(Core::null{}),
                    nullptr);

                // TODO: .define() if defined

                body_env->declare_variable(std::move(p_variable));
            }

            fn->parameters.push_back(std::move(parameter));
        }
    }

    void validate_duplication(Analyzer &analyzer,
                              std::unique_ptr<Core::FunctionSymbol> &fn,
                              AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();
        AST::FunctionDeclarationStmt *node = fn->node;

        auto declared = current->resolve_symbol(fn->name);
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

        // Declared and defined; or
        // Declared and attempting to re-declare
        if (defined || !is_def)
        {
            error(std::string("Cannot re-declare symbol \"") +
                  Diagnostic::ERR_GEN + declared->name + Utils::Styles::Reset +
                  "\".");

            return;
        }

        auto declptr = dynamic_cast<Core::FunctionSymbol *>(declared);
        if (declptr == nullptr)
        {
            error(std::string("Cannot provide definition for non-function "
                              "declaration of \"") +
                  Diagnostic::ERR_GEN + declared->name + Utils::Styles::Reset +
                  "\".");

            return;
        }

        // Check for parameter list difference
        auto err_message = std::string("Cannot define function \"") +
                           Diagnostic::ERR_GEN + declared->name +
                           Utils::Styles::Reset +
                           "\". Function signature mismatch.";

        auto &declared_params = declptr->parameters,
             curr_params = fn->parameters;

        size_t dp_size = declared_params.size();
        if (dp_size != curr_params.size())
        {
            error(std::move(err_message));
            return;
        }

        for (size_t i = 0; i < dp_size; i++)
        {
            if (declared_params[i] == curr_params[i])
                continue;

            error(std::move(err_message));
            break;
        }
    }

    AnalysisResult AnalyzerImpl<AST::FunctionDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::FunctionDeclarationStmt &node)
    {
        Core::Environment *closure = analyzer.current_env(), *current = closure;
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        auto function = std::make_unique<Core::FunctionSymbol>(
            std::string(node.name().value), node.position(),
            current->resolve_type(
                std::string(node.return_type().value().value)),
            std::vector<Core::FunctionParameter>{}, &node);

        auto is_def = node.is_definition();
        Core::Environment *body_env =
            is_def ? &current->create_child() : nullptr;

        // Invalid Return Type
        if (function->return_type == nullptr)
            result.error(
                Diagnostic::create_unknown_type_error(&node.return_type()));

        result.data = function->return_type;

        validate_duplication(analyzer, function, result);

        validate_parameters(analyzer, function, result, body_env);

        // Analyze the body if the statement is a definition
        if (is_def)
        {
            current = body_env;
            analyzer.set_current_env(*body_env);

            if (auto ptr = dynamic_cast<AST::FunctionDefinitionStmt *>(&node))
            {
                function->define(
                    const_cast<Core::Position *>(&ptr->body().position()));

                for (auto &stmt : ptr->body().statements())
                {
                    AnalysisResult a_res =
                        AnalyzerImpl<AST::Stmt>::analyze(analyzer, *stmt);

                    result.adapt(a_res.status, std::move(a_res.diagnostics));
                }
            }
        }

        closure->declare_symbol(std::move(function));

        return result;
    }

} // namespace Semantic
