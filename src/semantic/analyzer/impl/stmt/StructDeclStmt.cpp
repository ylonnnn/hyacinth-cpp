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

        auto error = [&](Core::Symbol *symbol,
                         const std::string &message) -> void
        {
            auto err_node = AST::LiteralExpr(st_name);
            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &err_node, Diagnostic::ErrorTypes::Semantic::Duplication,
                std::move(message),
                "Identifier \"" + identifier + "\" is already used");

            auto defined = symbol->defined_at != nullptr;

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                symbol->node,
                defined ? Diagnostic::NoteType::Definition
                        : Diagnostic::NoteType::Declaration,
                std::string("A symbol identified as \"") +
                    Diagnostic::NOTE_GEN + identifier + Utils::Styles::Reset +
                    "\" is already declared.",
                "Declared here"));

            result.error(std::move(diagnostic));
        };

        // Shadowing Check
        // Start at parent to lessen iteration
        Core::BaseType *d_resolved = current->resolve_type(identifier);
        if (d_resolved != nullptr && d_resolved->builtin())
        {
            auto n_node = AST::LiteralExpr(node.name());
            result.error(&n_node,
                         Diagnostic::ErrorTypes::Semantic::IllegalShadowing,
                         std::string("Illegal shadowing of built-in type \"") +
                             Diagnostic::ERR_GEN + identifier +
                             Utils::Styles::Reset + "\".",
                         "Cannot shadow built-in types");
            return {nullptr, false};
        }

        auto depth = static_cast<size_t>(Core::ResolutionType::Current);
        Core::BaseType *resolved = current->resolve_type(identifier, depth);

        if (resolved == nullptr)
            return {nullptr, true};

        Core::TypeSymbol *declared = resolved->symbol();
        auto declptr = dynamic_cast<Core::StructSymbol *>(declared);

        if (declptr == nullptr)
        {
            error(declared,
                  std::string("Cannot provide definition for non-struct"
                              " declaration of \"") +
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
            error(declared, std::string("Cannot re-declare symbol \"") +
                                Diagnostic::ERR_GEN + identifier +
                                Utils::Styles::Reset + "\".");

            return {declptr, false};
        }

        return {declptr, true};
    }

    static void analyze_fields(Analyzer &analyzer, Core::StructSymbol *struct_,
                               AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();
        auto def = struct_->definition;

        auto &members = struct_->type->members();

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

            members.try_emplace(std::string(name),
                                Core::TypeMember{t_res.data});

            struct_->type->field_count()++;
        }
    }

    AnalysisResult AnalyzerImpl<AST::StructDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::StructDeclarationStmt &node)
    {
        Core::Environment *current = analyzer.current_env();
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};

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
                std::unordered_map<std::string_view, Core::Type *>{}, declared);
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
