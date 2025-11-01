#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/variable/VariableDefStmt.hpp"
// #include "core/symbol/VariableSymbol.hpp"
// #include "core/type/Type.hpp"
// #include "core/type/primitive/Void.hpp"
// #include "core/value/Value.hpp"
#include "analyzer/Analyzer.hpp"
#include "analyzer/impl/Stmt.hpp"
#include "ast/type/Type.hpp"
#include "core/symbol/TypeSymbol.hpp"
#include "core/symbol/VariableSymbol.hpp"
#include "utils/dev.hpp"

namespace Semantic
{
    namespace VariableAnalyzer
    {
        void verify_duplication(Analyzer &analyzer,
                                Core::VariableSymbol *varsym,
                                AnalysisResult &result)
        {
            Core::Environment *root = analyzer.env_stack.root(),
                              *current = analyzer.env_stack.current();

            AST::VariableDeclarationStmt *node = varsym->decl_node;

            std::string ident(node->name.value);

            // Built-in Type Verification
            if (auto symbol = root->resolve_symbol(ident); symbol != nullptr)
            {
                // If the symbol is not a type symbol
                if (typeid(*symbol) != typeid(Core::TypeSymbol))
                    return;

                // If the declaration node of the symbol exists, it is not
                // built-in
                if (symbol->decl_node != nullptr)
                    return;

                result.error(Core::PositionRange(node->name.range),
                             Diagnostic::ErrorType::IllegalShadowing,
                             "illegal shadowing of built-in type '" +
                                 std::string(symbol->name) + "'.");

                return;
            }

            Core::Symbol *declared = current->resolve_symbol(
                ident, Core::EnvironmentResolutionType::Current);

            std::cout << "declared: " << declared << "\n";
            if (declared == nullptr)
                return;

            result.error(Core::PositionRange(node->name.range),
                         Diagnostic::ErrorType::IdentifierConflict,
                         "identifier '" + ident + "' is already used.");

            auto is_defined = declared->def_node != nullptr,
                 is_definition = node->is_definition();

            if (is_defined || is_definition)
            {
                auto decl = declared->decl_node;
                std::cout << "re-declaration\n";

                result
                    .error(Core::PositionRange(node->name.range),
                           Diagnostic::ErrorType::IllegalVariableRedeclaration,
                           "cannot re-declare variable '" + ident + "'.")
                    ->add_detail(
                        Diagnostic::DiagnosticSeverity::Note,
                        static_cast<uint32_t>(Diagnostic::NoteType::Info),
                        Core::PositionRange{decl->position,
                                            *decl->end_position},
                        "variable '" + ident + "' is already declared here");

                return;
            }
        }

        bool analyze_type(Analyzer &analyzer, Core::VariableSymbol *varsym,
                          AnalysisResult &result)
        {
            Core::Environment *current = analyzer.env_stack.current();

            AST::Type *ast_type = varsym->decl_node->type.get();
            if (ast_type == nullptr)
                return true;

            AnalysisResult t_res =
                AnalyzerImpl<AST::Type>::analyze(analyzer, *ast_type);

            result.adapt(t_res);
            varsym->type = result.data;

            return true;

            // result.error(ast_type,
            // Diagnostic::ErrorTypes::Type::InvalidVariableType,
            //              std::string("Type \"") + Diagnostic::ERR_GEN
            //              +
            //                  ast_type->to_string() +
            //                  utils::Styles::Reset +
            //                  "\" can only be used as a function
            //                  return type.",
            //              "Only functions can have return types of
            //              this type");
        }

        void analyze_value([[maybe_unused]] Analyzer &analyzer,
                           Core::VariableSymbol *varsym, AnalysisResult &result)
        {
            auto ptr =
                dynamic_cast<AST::VariableDefinitionStmt *>(varsym->decl_node);
            if (ptr == nullptr)
                return;

            AnalysisResult v_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, *ptr->value);

            result.adapt(v_res);
            Core::Value *val = result.value;

            // Validate type
            if (varsym->type != nullptr)
            {
                Core::TypeResult vv_res = varsym->type->assignable(val);
                result.adapt(vv_res.status, std::move(vv_res.diagnostics));

                if (result.status == Core::ResultStatus::Fail)
                    return;
            }

            // Otherwise, infer type
            else
            {
                utils::todo("implement type inference");
                return;
            }

            varsym->value = v_res.value;

            //     // Define
            //     var->define(const_cast<Core::Position *>(&stmt.position()));
            //     var->value = std::move(v_res.value);

            //     var->node->set_value(var->value);

            //     // If type is not specified, it is inferred
            //     // No type validation required
            //     if (ast_type == nullptr)
            //     {
            //         result.data = v_res.data;
            //         var->type = v_res.data;

            //         return;
            //     }

            //     if (!result.data->assignable_with(*var->type) ||
            //         (var->value != nullptr &&
            //         !result.data->assignable(*var->value))) error();
        }

    } // namespace VariableAnalyzer

    AnalysisResult AnalyzerImpl<AST::VariableDeclarationStmt>::analyze(
        Analyzer &analyzer, AST::VariableDeclarationStmt &node)
    {
        // Core::Environment *current = analyzer.current_env();
        AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        Core::Environment *current = analyzer.env_stack.current();

        auto varsym = std::make_unique<Core::VariableSymbol>(
            node.name.value,
            node.accessibility == AST::DeclarationAccessibility::Public
                ? Core::SymbolAccessibility::Public
                : Core::SymbolAccessibility::Private,
            &node, node.is_mutable(), nullptr, nullptr);

        if (node.is_definition())
            varsym->define(&node);

        Core::VariableSymbol *varptr = varsym.get();

        VariableAnalyzer::verify_duplication(analyzer, varptr, result);

        if (!VariableAnalyzer::analyze_type(analyzer, varptr, result))
            return result;

        // If the node is a variable definition
        if (node.is_definition())
            VariableAnalyzer::analyze_value(analyzer, varptr, result);

        // For variable declaration with no type annotation provided
        else if (node.type == nullptr)
        {
            result.error(Core::PositionRange{node.position, *node.end_position},
                         Diagnostic::ErrorType::InvalidVariableDeclaration,
                         "cannot declare a variable with neither explicit type "
                         "annotation nor value initializer.");

            return result;
        }

        //                     Diagnostic::ErrorTypes::Type::InvalidVariableType,
        //                     std::string("Variables must either have
        //                     explicit
        //                     ") +
        //                         Diagnostic::ERR_GEN + "TYPE" +
        //                         utils::Styles::Reset + " or " +
        //                         Diagnostic::ERR_GEN + "VALUE" +
        //                         utils::Styles::Reset + ".",
        //                     "No explicit type nor value provided");

        //             return result;
        // }

        std::cout << varsym->name << " added\n";
        current->add_symbol(std::move(varsym));

        return result;
    }

} // namespace Semantic
