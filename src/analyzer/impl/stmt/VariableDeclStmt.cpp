#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "analyzer/Analyzer.hpp"
#include "analyzer/impl/Stmt.hpp"
#include "ast/stmt/variable/VariableDefStmt.hpp"
#include "ast/type/Type.hpp"
#include "core/symbol/TypeSymbol.hpp"
#include "core/symbol/VariableSymbol.hpp"
#include "core/type/wrapper/PointerType.hpp"
#include "core/type/wrapper/ReferenceType.hpp"
#include "core/value/Value.hpp"
#include "core/value/ValuePool.hpp"

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
            auto bi_type_ver = [root, &ident, &result, &node]() -> bool
            {
                auto symbol = root->resolve_symbol(ident);
                if (symbol == nullptr)
                    return true;

                // If the symbol is not a type symbol
                if (typeid(*symbol) != typeid(Core::TypeSymbol))
                    return true;

                // If the declaration node of the symbol exists, it is not
                // built-in
                if (symbol->decl_node != nullptr)
                    return true;

                result.error(node->name.range,
                             Diagnostic::ErrorType::IllegalShadowing,
                             "illegal shadowing of built-in type '" +
                                 std::string(symbol->name) + "'.");

                return false;
            };

            Core::Symbol *declared = current->resolve_symbol(
                ident, Core::EnvironmentResolutionType::Current);

            // If the symbol is not declared, or if the declaration shadows a
            // built-in type, return and let the other handlers handle it
            if (declared == nullptr || !bi_type_ver())
                return;

            // result.error(Core::PositionRange(node->name.range),
            //              Diagnostic::ErrorType::IdentifierConflict,
            //              "identifier '" + ident + "' is already
            //              used.");

            auto is_defined = declared->def_node != nullptr,
                 is_definition = node->is_definition();

            if (is_defined || is_definition)
            {
                auto decl = declared->decl_node;
                std::cout << "re-declaration\n";

                result
                    .error(node->name.range,
                           Diagnostic::ErrorType::IllegalVariableRedeclaration,
                           "cannot re-declare variable '" + ident + "'.")
                    ->add_detail(
                        Diagnostic::DiagnosticSeverity::Note,
                        static_cast<uint32_t>(Diagnostic::NoteType::Info),
                        decl->range,
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

            // TODO: Handle Void Type

            return true;
        }

        // TODO: Move to Type::assignment_semantic
        void determine_assignment(Core::VariableSymbol *varsym,
                                  AnalysisResult &result)
        {
            Core::Value *val = result.value;
            if (val == nullptr) [[unlikely]]
                return;

            Core::InstantiatedType *type = result.data;

            if (typeid(*type) == typeid(Core::ReferenceInstantiated))
            {
                auto ref = static_cast<Core::ReferenceInstantiated *>(type);
                varsym->value = val;
            }

            else if (typeid(*type) == typeid(Core::PointerInstantiated))
            {
                auto ptr = static_cast<Core::PointerInstantiated *>(type);
                // TODO: Handle Pointer Values
            }

            // Create a copy
            else
            {
                Core::ReadValue *rv_copy = Core::create_copy(*val);
                varsym->value = rv_copy;
            }
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

            if (val == nullptr)
            {
                if (ptr->is_constexpr)
                    result.error(&ptr->range,
                                 Diagnostic::ErrorType::NonConstantEvaluatable,
                                 "constexpr variables must have compile-time "
                                 "evaluatable values.");

                return;
            }

            // Type Inferrence
            if (varsym->type == nullptr)
            {
                varsym->type =
                    Core::BaseType::infer(*analyzer.env_stack.current(), *val);

                if (ptr->is_constexpr)
                    varsym->value = Core::create_copy(*val);
            }

            // Type Validation
            else
            {
                Core::TypeResult vv_res = varsym->type->assignable(val);
                result.adapt(vv_res.status, std::move(vv_res.diagnostics));

                if (ptr->is_constexpr)
                    varsym->value = varsym->type->base.transfer_semantics(val);
            }
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
        VariableAnalyzer::analyze_type(analyzer, varptr, result);

        // If the node is a variable definition
        if (node.is_definition())
            VariableAnalyzer::analyze_value(analyzer, varptr, result);

        // For variable declaration with no type annotation provided
        else if (node.type == nullptr)
        {
            result.error(node.range,
                         Diagnostic::ErrorType::InvalidVariableDeclaration,
                         "cannot declare a variable with neither explicit type "
                         "annotation nor value initializer.");

            return result;
        }

        std::cout << varsym->name << " added\n";
        current->add_symbol(std::move(varsym));

        return result;
    }

} // namespace Semantic
