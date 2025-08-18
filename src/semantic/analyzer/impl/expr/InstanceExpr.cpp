#include "core/type/compound/Struct.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "semantic/analyzer/impl/Expr.hpp"
#include <memory>

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::InstanceExpr>::analyze(Analyzer &analyzer,
                                             AST::InstanceExpr &node)
    {
        AnalysisResult result = {std::make_shared<Core::Value>(Core::object{}),
                                 Core::ResultStatus::Success,
                                 nullptr,
                                 {}};

        Core::Environment *current = analyzer.current_env();
        auto obj = std::get_if<Core::object>(result.value.get());

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

        auto *members = type == nullptr ? nullptr : &type->members();
        for (auto &[name, field] : node.fields())
        {
            AST::Expr &f_val = field.value();

            AnalysisResult v_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, f_val);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            if (members != nullptr)
            {
                auto it = members->find(name);
                if (it == members->end())
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

                auto type = it->second.as<Core::Type>();
                if (type == nullptr)
                {
                    // TODO: Non-field member error
                    // result.error();
                    return result;
                }

                if (!(v_res.value && type->assignable(*v_res.value)) &&
                    !(v_res.data != nullptr &&
                      type->assignable_with(*v_res.data)))
                {
                    result.error(
                        &f_val, Diagnostic::ErrorTypes::Type::Mismatch,
                        std::string("Field \"") + Diagnostic::ERR_GEN + name +
                            Utils::Styles::Reset + "\" has a value of type \"" +
                            Diagnostic::ERR_GEN + v_res.data->to_string() +
                            Utils::Styles::Reset +
                            "\". Expects values of type \"" +
                            Diagnostic::ERR_GEN + type->to_string() +
                            Utils::Styles::Reset + "\".",
                        "Field type mismatch");

                    return result;
                }
            }

            obj->set(name,
                     Core::value_data{std::move(v_res.value), v_res.data});
        }

        result.value = std::make_shared<Core::Value>(*obj);

        return result;
    }

} // namespace Semantic
