#include "core/symbol/FunctionSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    FunctionParameterSymbol::FunctionParameterSymbol(
        std::string_view name, Core::Position &declared_at, bool is_mutable,
        Type *type, std::optional<Value> value, AST::FunctionParameter *node)
        : IdentifierSymbol(name, declared_at, is_mutable, type,
                           std::move(value), node)
    {
        this->type = type;
        this->node = dynamic_cast<AST::FunctionParameter *>(Symbol::node);
    }

    FunctionSymbol::FunctionSymbol(std::string_view name,
                                   SymbolAccessibility accessibility,
                                   Core::Position &declared_at,
                                   Type *return_type,
                                   std::vector<FunctionParameter> &&parameters,
                                   AST::FunctionDeclarationStmt *node)
        : Symbol(name, accessibility, declared_at, node),
          return_type(std::move(return_type)), parameters(std::move(parameters))
    {
        this->node = dynamic_cast<AST::FunctionDeclarationStmt *>(node);
        if (typeid(*node) == typeid(AST::FunctionDefinitionStmt))
            define(static_cast<AST::FunctionDefinitionStmt *>(node));
    }

    void FunctionSymbol::construct_signature()
    {
        signature += std::string(name) + "->" + return_type->to_string() + "(";

        for (auto &param : parameters)
        {
            auto &[_, is_mutable, type] = param;
            signature += (is_mutable ? "!:" : ":") + type->to_string();

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
