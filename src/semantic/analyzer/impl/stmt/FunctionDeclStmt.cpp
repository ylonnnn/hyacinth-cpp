#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "core/environment/FunctionEnvironment.hpp"
#include "core/symbol/FunctionSymbol.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    static void analyze_return_type(Analyzer &analyzer,
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

        fn->return_type = result.data = t_res.data;
    }

    static void analyze_parameters(Analyzer &analyzer,
                                   std::unique_ptr<Core::FunctionSymbol> &fn,
                                   AnalysisResult &result,
                                   Core::Environment *body_env)
    {
        Core::Environment *current = analyzer.current_env();
        AST::FunctionDeclarationStmt *node = fn->node;

        auto is_def = node->is_definition();

        for (auto &param : node->parameters())
        {
            auto &ast_type = param.type();
            Core::BaseType *resolved =
                current->resolve_type(std::string(ast_type.value().value));

            if (resolved == nullptr)
            {
                result.error(Diagnostic::create_unknown_type_error(&ast_type));
                continue;
            }

            Core::TypeResolutionResult t_res = resolved->resolve(ast_type);
            result.adapt(t_res.status, std::move(t_res.diagnostics));

            Core::FunctionParameter parameter{param.name().value,
                                              param.is_mutable(), t_res.data};

            // Register/Declare the parameters to the environment for analysis
            if (is_def)
            {
                auto p_symbol = std::make_unique<Core::FunctionParameterSymbol>(
                    param.name().value, param.position(), param.is_mutable(),
                    parameter.type, nullptr, &param);

                // TODO: .define() for defaulted parameters

                body_env->declare_symbol(std::move(p_symbol));
            }

            fn->parameters.push_back(std::move(parameter));
        }
    }

    static std::pair<Core::FunctionSymbol *, bool>
    validate_duplication(Analyzer &analyzer,
                         std::unique_ptr<Core::FunctionSymbol> &fn,
                         AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();
        AST::FunctionDeclarationStmt *node = fn->node;

        std::string identifier(std::string(fn->name));

        // Shadowing Check
        // Start at parent to lessen iteration
        Core::BaseType *d_resolved = current->resolve_type(identifier);
        if (d_resolved != nullptr && d_resolved->builtin())
        {
            auto n_node = AST::LiteralExpr(node->name());
            result.error(&n_node,
                         Diagnostic::ErrorTypes::Semantic::IllegalShadowing,
                         std::string("Illegal shadowing of built-in type \"") +
                             Diagnostic::ERR_GEN + identifier +
                             Utils::Styles::Reset + "\".",
                         "Cannot shadow built-in types");

            return {nullptr, false};
        }

        auto depth = static_cast<size_t>(Core::ResolutionType::Current);
        auto declared = current->resolve_symbol(identifier, depth);

        if (declared == nullptr)
            return {nullptr, true};

        std::string name(declared->name);
        auto defined = declared->defined_at != nullptr,
             is_def = node->is_definition();

        auto error = [&](const std::string &message) -> void
        {
            auto err_node = AST::LiteralExpr(node->name());
            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &err_node, Diagnostic::ErrorTypes::Semantic::Duplication,
                std::move(message),
                "Identifier \"" + name + "\" is already used");

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                declared->node,
                defined ? Diagnostic::NoteType::Definition
                        : Diagnostic::NoteType::Declaration,
                std::string("A symbol identified as \"") +
                    Diagnostic::NOTE_GEN + name + Utils::Styles::Reset +
                    "\" is already declared.",
                "Declared here"));

            result.error(std::move(diagnostic));
        };

        auto declptr = dynamic_cast<Core::FunctionSymbol *>(declared);
        if (declptr == nullptr)
        {
            error(std::string("Cannot provide definition for non-function "
                              "declaration of \"") +
                  Diagnostic::ERR_GEN + name + Utils::Styles::Reset + "\".");

            return {nullptr, false};
        }

        // Declared and defined; or
        // Declared and attempting to re-declare
        if (defined || !is_def)
        {
            error(std::string("Cannot re-declare symbol \"") +
                  Diagnostic::ERR_GEN + name + Utils::Styles::Reset + "\".");

            return {declptr, false};
        }

        // Check for parameter list difference
        auto err_message = std::string("Cannot define function \"") +
                           Diagnostic::ERR_GEN + name + Utils::Styles::Reset +
                           "\". Function signature mismatch.";

        // Function Signatures
        if (fn->signature != declptr->signature)
        {
            error(std::move(err_message));
            return {declptr, false};
        }

        return {declptr, true};
    }

    static void analyze_body(Analyzer &analyzer,
                             std::unique_ptr<Core::FunctionSymbol> &fn,
                             AnalysisResult &result)
    {
        AST::FunctionDeclarationStmt *node = fn->node;

        if (typeid(*node) != typeid(AST::FunctionDefinitionStmt))
            return;

        auto defptr = static_cast<AST::FunctionDefinitionStmt *>(node);
        AST::BlockStmt &body = defptr->body();

        std::vector<std::unique_ptr<AST::Stmt>> &statements = body.statements();
        size_t r_size = statements.size() * 2;

        if (result.diagnostics.size() - r_size < result.diagnostics.capacity())
            result.diagnostics.reserve(r_size);

        for (auto &stmt : statements)
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
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        result.diagnostics.reserve(8);

        auto function = std::make_unique<Core::FunctionSymbol>(
            node.name().value,
            node.accessibility() == AST::DeclarationAccessibility::Public
                ? Core::SymbolAccessibility::Public
                : Core::SymbolAccessibility::Private,
            node.position(), nullptr, std::vector<Core::FunctionParameter>{},
            &node);

        auto is_def = node.is_definition();

        // Create function environment if the node is a function definition
        if (is_def)
        {
            auto &children = current->children();

            children.push_back(std::make_unique<Core::FunctionEnvironment>(
                current, function.get()));
            function->environment = children.back().get();
        }

        auto fn_env = function->environment;

        analyze_return_type(analyzer, function, result);
        analyze_parameters(analyzer, function, result, fn_env);

        function->construct_signature();

        auto [declared, valid] =
            validate_duplication(analyzer, function, result);
        if (!valid)
            return result;

        auto is_decl = declared != nullptr;
        declared = is_decl ? declared : function.get();

        // Analyze the body if the statement is a definition
        if (is_def)
        {
            declared->define(static_cast<AST::FunctionDefinitionStmt *>(&node));

            // Enter function body environment
            analyzer.set_current_env(*fn_env);
            analyze_body(analyzer, function, result);
            analyzer.set_current_env(*fn_env->parent());
        }

        if (!is_decl)
            closure->declare_symbol(std::move(function));

        return result;
    }

} // namespace Semantic
