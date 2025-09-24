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

        AST::Type &ast_type = node.type();

        Core::TypeResolutionResult t_res = current->resolve_ast_type(ast_type);
        result.adapt(t_res.status, std::move(t_res.diagnostics), t_res.data);

        Core::StructType *type =
            dynamic_cast<Core::StructType *>(result.data->type);
        if (type == nullptr)
        {
            result.error(
                &ast_type,
                Diagnostic::ErrorTypes::Type::InvalidInstantiationType,
                std::string("Cannot instantiate type \"") +
                    Diagnostic::ERR_GEN + result.data->to_string() +
                    utils::Styles::Reset + "\".",
                "Instantiation with the provided type is not possible");

            return result;
        }

        auto &fields = node.fields();
        auto expected = type->field_count();

        if (expected != fields.size())
        {
            result.error(Diagnostic::create_invalid_entry_count_error(
                &node, Diagnostic::ErrorTypes::Semantic::InvalidFieldCount,
                "field", expected, fields.size()));

            return result;
        }

        obj->type() = result.data;
        auto *members = &type->members();

        auto bind = true;

        for (auto &[name, field] : fields)
        {
            AST::Expr &f_val = field.value();

            AnalysisResult v_res =
                AnalyzerImpl<AST::Expr>::analyze(analyzer, f_val);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            auto it = members->find(name);
            if (it == members->end())
            {
                result.error(
                    &field, Diagnostic::ErrorTypes::Semantic::UnrecognizedField,
                    std::string("Unrecognized field \"") + Diagnostic::ERR_GEN +
                        name + utils::Styles::Reset + "\" provided.",
                    "\"" + name + "\" is not a \"" + std::string(type->name()) +
                        "\" field");

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
                !(v_res.data != nullptr && type->assignable_with(*v_res.data)))
            {
                result.error(
                    &f_val, Diagnostic::ErrorTypes::Type::Mismatch,
                    std::string("Field \"") + Diagnostic::ERR_GEN + name +
                        utils::Styles::Reset + "\" has a value of type \"" +
                        Diagnostic::ERR_GEN + v_res.data->to_string() +
                        utils::Styles::Reset + "\". Expects values of type \"" +
                        Diagnostic::ERR_GEN + type->to_string() +
                        utils::Styles::Reset + "\".",
                    "Field type mismatch");

                return result;
            }

            if (v_res.value == nullptr)
                bind = false;

            obj->set(name,
                     Core::value_data{std::move(v_res.value), v_res.data});
        }

        result.value = std::make_shared<Core::Value>(*obj);

        if (bind)
            node.set_value(result.value);

        return result;
    }

} // namespace Semantic
