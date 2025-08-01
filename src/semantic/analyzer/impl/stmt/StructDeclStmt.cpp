#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "core/symbol/type/StructSymbol.hpp"
#include "core/type/compound/Struct.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    static std::pair<Core::StructSymbol *, bool>
    validate_duplication(Analyzer &analyzer, AST::StructDeclarationStmt &node,
                         AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();

        Lexer::Token &st_name = node.name();
        std::string identifier(st_name.value);

        Core::Symbol *declared = current->resolve_symbol(identifier);

        auto error = [&](const std::string &message) -> void
        {
            auto err_node = AST::LiteralExpr(st_name);
            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &err_node, Diagnostic::ErrorTypes::Semantic::Duplication,
                std::move(message),
                "Identifier \"" + identifier + "\" is already used");

            auto defined = declared->defined_at != nullptr;

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                declared->node,
                defined ? Diagnostic::NoteType::Definition
                        : Diagnostic::NoteType::Declaration,
                std::string("A symbol identified as \"") +
                    Diagnostic::NOTE_GEN + identifier + Utils::Styles::Reset +
                    "\" is already declared.",
                "Declared here"));

            result.error(std::move(diagnostic));
        };

        if (declared == nullptr)
        {
            auto valid = current->resolve_type(identifier) == nullptr;
            if (!valid)
                error(std::string("Cannot re-declare symbol \"") +
                      Diagnostic::ERR_GEN + identifier + Utils::Styles::Reset +
                      "\".");

            return {nullptr, valid};
        }

        auto declptr = dynamic_cast<Core::StructSymbol *>(declared);
        if (declptr == nullptr)
        {
            error(std::string("Cannot provide definition for non-struct"
                              "declaration of \"") +
                  Diagnostic::ERR_GEN + identifier + Utils::Styles::Reset +
                  "\".");

            return {nullptr, false};
        }

        auto defined = declared->defined_at != nullptr,
             is_def = node.is_definition();

        // Declared and defined; or
        // Declared and attempting to re-declare
        if (defined || !is_def)
        {
            error(std::string("Cannot re-declare symbol \"") +
                  Diagnostic::ERR_GEN + identifier + Utils::Styles::Reset +
                  "\".");

            return {declptr, false};
        }

        return {declptr, true};
    }

    static void analyze_fields(Analyzer &analyzer, Core::StructSymbol *struct_,
                               AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();
        auto def = struct_->definition;

        for (auto &[name, field] : def->fields())
        {
            AST::Type &ast_type = field.type();

            Core::BaseType *resolved =
                current->resolve_type(std::string(ast_type.value().value));
            if (resolved == nullptr)
            {
                result.error(Diagnostic::create_unknown_type_error(&ast_type));
                continue;
            }

            Core::TypeResolutionResult t_res = resolved->resolve(ast_type);
            result.adapt(t_res.status, std::move(t_res.diagnostics));

            struct_->type->fields().try_emplace(name, std::move(t_res.data));
        }
    }

    AnalysisResult AnalyzerImpl<AST::StructDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::StructDeclarationStmt &node)
    {
        Core::Environment *current = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        result.diagnostics.reserve(8);

        auto [declared, valid] = validate_duplication(analyzer, node, result);
        if (!valid)
            return result;

        auto is_decl = declared != nullptr;
        if (!is_decl)
        {
            declared = new Core::StructSymbol(
                node.name().value,
                node.accessibility() == AST::DeclarationAccessibility::Public
                    ? Core::SymbolAccessibility::Public
                    : Core::SymbolAccessibility::Private,
                node.position(), &node);

            declared->type = new Core::StructType(
                current, declared->name,
                std::unordered_map<std::string_view,
                                   std::unique_ptr<Core::Type>>{});
        }

        if (node.is_definition())
        {
            declared->define(static_cast<AST::StructDefinitionStmt *>(&node));
            analyze_fields(analyzer, declared, result);
        }

        current->declare_type(
            std::unique_ptr<Core::StructType>(declared->type));

        if (!is_decl)
            current->declare_symbol(
                std::unique_ptr<Core::StructSymbol>(declared));

        return result;
    }

} // namespace Semantic
