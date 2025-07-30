#include "core/symbol/FunctionSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    FunctionParameterSymbol::FunctionParameterSymbol(
        std::string_view name, Core::Position declared_at, bool is_mutable,
        Type *type, std::optional<Value> value, AST::FunctionParameter *node)
        : IdentifierSymbol(name, declared_at, is_mutable, nullptr,
                           std::move(value), node)
    {
        this->type = type;
        this->node = dynamic_cast<AST::FunctionParameter *>(Symbol::node);
    }

    Type *FunctionParameterSymbol::type_() { return type; }

    FunctionSymbol::FunctionSymbol(std::string_view name,
                                   Core::Position declared_at,
                                   std::unique_ptr<Type> return_type,
                                   std::vector<FunctionParameter> &&parameters,
                                   AST::FunctionDeclarationStmt *node)
        : Symbol(std::move(name), std::move(declared_at), node),
          return_type(std::move(return_type)), parameters(std::move(parameters))
    {
        this->node = dynamic_cast<AST::FunctionDeclarationStmt *>(Symbol::node);
    }

    void FunctionSymbol::construct_signature()
    {
        signature += std::string(name) + "->" + return_type->to_string() + "(";

        for (auto &param : parameters)
        {
            auto &[name, is_mutable, type] = param;
            signature += std::string(name) + (is_mutable ? "!:" : ":") +
                         type->to_string();

            if (&param != &parameters.back())
                signature += ",";
        }

        signature += ")";
    }

    void FunctionSymbol::define(Core::Position *position)
    {
        defined_at = position;
    }

    void FunctionSymbol::define(AST::FunctionDefinitionStmt *definition)
    {
        define(&definition->position());
        this->definition = definition;
    }

} // namespace Core
