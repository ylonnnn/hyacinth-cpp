#include <optional>

#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "core/symbol/FunctionSymbol.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    void analyze_return_type(Analyzer &analyzer,
                             std::unique_ptr<Core::FunctionSymbol> &fn,
                             AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();
        AST::FunctionDeclarationStmt *node = fn->node;

        auto &ast_type = node->return_type();
        Core::BaseType *resolved =
            current->resolve_type(std::string(ast_type.value().value));

        if (resolved == nullptr)
        {
            result.error(Diagnostic::create_unknown_type_error(&ast_type));
            return;
        }

        Core::TypeResolutionResult t_res = resolved->resolve(ast_type);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        fn->return_type = std::move(t_res.data);
        result.data = fn->return_type.get();
    }

    void analyze_parameters(Analyzer &analyzer,
                            std::unique_ptr<Core::FunctionSymbol> &fn,
                            AnalysisResult &result, Core::Environment *body_env)
    {
        Core::Environment *current = analyzer.current_env();
        AST::FunctionDeclarationStmt *node = fn->node;

        auto is_def = node->is_definition();

        for (auto &param : node->parameters())
        {
            auto &ast_type = param->type();
            Core::BaseType *resolved =
                current->resolve_type(std::string(ast_type.value().value));

            if (resolved == nullptr)
            {
                result.error(Diagnostic::create_unknown_type_error(&ast_type));
                continue;
            }

            Core::TypeResolutionResult t_res = resolved->resolve(ast_type);
            result.adapt(t_res.status, std::move(t_res.diagnostics));

            Core::FunctionParameter parameter{param->name().value,
                                              param->is_mutable(),
                                              std::move(t_res.data)};

            // Register/Declare the parameters as variables to the environment
            // for analysis
            if (is_def)
            {
                // auto p_variable = std::make_unique<Core::VariableSymbol>(
                //     std::string(param->name().value), param->position(),
                //     param->is_mutable(), nullptr, Core::Value(Core::null{}),
                //     nullptr);

                // // TODO: .define() if defined

                // body_env->declare_variable(std::move(p_variable));
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

        // Function Signatures
        if (fn->signature != declptr->signature)
        {
            error(std::move(err_message));
            return;
        }
    }

    void analyze_body(Analyzer &analyzer,
                      std::unique_ptr<Core::FunctionSymbol> &fn,
                      AnalysisResult &result)
    {
        AST::FunctionDeclarationStmt *node = fn->node;

        if (typeid(*node) != typeid(AST::FunctionDefinitionStmt))
            return;

        auto defptr = static_cast<AST::FunctionDefinitionStmt *>(node);
        AST::BlockStmt &body = defptr->body();

        fn->define(const_cast<Core::Position *>(&body.position()));

        for (auto &stmt : body.statements())
        {
            AnalysisResult a_res =
                AnalyzerImpl<AST::Stmt>::analyze(analyzer, *stmt);

            result.adapt(a_res.status, std::move(a_res.diagnostics));
        }
    }

    AnalysisResult AnalyzerImpl<AST::FunctionDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::FunctionDeclarationStmt &node)
    {
        Core::Environment *closure = analyzer.current_env(), *current = closure;
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        auto function = std::make_unique<Core::FunctionSymbol>(
            std::string(node.name().value), node.position(), nullptr,
            std::vector<Core::FunctionParameter>{}, &node);

        auto is_def = node.is_definition();
        Core::Environment *body_env =
            is_def ? &current->create_child() : nullptr;

        analyze_return_type(analyzer, function, result);
        analyze_parameters(analyzer, function, result, body_env);

        function->construct_signature();

        validate_duplication(analyzer, function, result);

        // Analyze the body if the statement is a definition
        if (is_def)
        {
            current = body_env;
            analyzer.set_current_env(*body_env);

            analyze_body(analyzer, function, result);
        }

        closure->declare_symbol(std::move(function));

        return result;
    }

} // namespace Semantic
