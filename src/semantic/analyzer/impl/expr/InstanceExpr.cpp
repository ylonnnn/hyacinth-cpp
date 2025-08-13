#include "core/type/compound/Struct.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "semantic/analyzer/impl/Expr.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::InstanceExpr>::analyze(Analyzer &analyzer,
                                             AST::InstanceExpr &node)
    {
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        Core::Environment *current = analyzer.current_env();
        Core::object obj;
        Core::StructType *type = nullptr;

        AST::Type *ast_type = node.type();
        if (ast_type != nullptr)
        {

            Core::BaseType *resolved =
                current->resolve_type(std::string(ast_type->value().value));
            if (resolved == nullptr)
            {
                result.error(Diagnostic::create_unknown_type_error(ast_type));
                return result;
            }

            Core::TypeResolutionResult t_res = resolved->resolve(*ast_type);
            result.adapt(t_res.status, std::move(t_res.diagnostics),
                         t_res.data);

            type = dynamic_cast<Core::StructType *>(result.data->type);
            if (type == nullptr)
            {
                result.error(
                    ast_type,
                    Diagnostic::ErrorTypes::Type::InvalidInstantiationType,
                    std::string("Cannot instantiate type \"") +
                        Diagnostic::ERR_GEN + result.data->to_string() +
                        Utils::Styles::Reset + "\".",
                    "Instantiation with the provided type is not possible");

                return result;
            }
        }

        auto *fields = type == nullptr ? nullptr : &type->fields();
        for (auto &[name, field] : node.fields())
        {
            AnalysisResult v_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, field.value());
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            if (fields != nullptr)
            {
                auto it = fields->find(name);
                if (it == fields->end())
                {
                    result.error(
                        &field,
                        Diagnostic::ErrorTypes::Semantic::UnrecognizedField,
                        std::string("Unrecognized field \"") +
                            Diagnostic::ERR_GEN + name + Utils::Styles::Reset +
                            "\" provided.",
                        "\"" + name + "\" is not a \"" +
                            std::string(type->name()) + "\" field");

                    return result;
                }
            }

            obj.set(name,
                    Core::object_entry{std::move(v_res.value), v_res.data});
        }

        result.value = std::move(obj);

        return result;
    }

} // namespace Semantic
