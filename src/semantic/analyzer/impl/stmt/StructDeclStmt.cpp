#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "core/symbol/type/StructSymbol.hpp"
#include "core/type/compound/Struct.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    static bool
    validate_duplication(Analyzer &analyzer,
                         std::unique_ptr<Core::StructSymbol> &struct_,
                         AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();
        AST::StructDeclarationStmt *node = struct_->node;

        std::string identifier = std::string(node->name().value);
        Core::Symbol *declared = current->resolve_symbol(identifier);

        auto error = [&](const std::string &message) -> void
        {
            auto err_node = AST::LiteralExpr(node->name());
            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &err_node, Diagnostic::ErrorTypes::Semantic::Duplication,
                std::move(message),
                "Identifier \"" + declared->name + "\" is already used");

            auto defined = struct_->defined_at != nullptr;

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

        std::cout << "declared: " << identifier << ": " << (declared == nullptr)
                  << "\n";
        if (declared == nullptr)
        {
            auto valid = current->resolve_type(identifier) == nullptr;
            if (current->resolve_type(identifier) != nullptr)
                error(std::string("Cannot re-declare symbol \"") +
                      Diagnostic::ERR_GEN + declared->name +
                      Utils::Styles::Reset + "\".");

            return valid;
        }

        auto defined = declared->defined_at != nullptr,
             is_def = node->is_definition();

        // Declared and defined; or
        // Declared and attempting to re-declare
        if (defined || !is_def)
        {
            error(std::string("Cannot re-declare symbol \"") +
                  Diagnostic::ERR_GEN + declared->name + Utils::Styles::Reset +
                  "\".");

            return false;
        }

        auto declptr = dynamic_cast<Core::StructSymbol *>(declared);
        if (declptr == nullptr)
        {
            error(std::string("Cannot provide definition for non-struct"
                              "declaration of \"") +
                  Diagnostic::ERR_GEN + declared->name + Utils::Styles::Reset +
                  "\".");

            return false;
        }

        return true;
    }

    static void analyze_fields(Analyzer &analyzer,
                               std::unique_ptr<Core::StructSymbol> &struct_,
                               AnalysisResult &result)
    {
        Core::Environment *current = analyzer.current_env();
        auto def = static_cast<AST::StructDefinitionStmt *>(struct_->node);

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

            struct_->fields.try_emplace(name, std::move(t_res.data));
        }
    }

    AnalysisResult AnalyzerImpl<AST::StructDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::StructDeclarationStmt &node)
    {
        Core::Environment *current = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        result.diagnostics.reserve(8);

        auto _struct = std::make_unique<Core::StructSymbol>(
            std::string(node.name().value), node.position(),
            std::vector<Core::StructField>{}, &node);

        if (!validate_duplication(analyzer, _struct, result))
            return result;

        if (node.is_definition())
        {
            _struct->define(const_cast<Core::Position *>(&node.position()));
            analyze_fields(analyzer, _struct, result);
        }

        current->declare_type(std::make_unique<Core::StructType>(
            current, node.name().value, std::move(_struct->fields)));
        current->declare_symbol(std::move(_struct));

        return result;
    }

} // namespace Semantic
