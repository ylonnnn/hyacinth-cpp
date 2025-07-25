#include "core/symbol/FunctionSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    // bool FunctionParameter::operator==(const FunctionParameter &other) const
    // {
    //     return name == other.name && is_mutable == other.is_mutable &&
    //            (*type == *other.type);
    // }

    FunctionSymbol::FunctionSymbol(std::string name, Core::Position declared_at,
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
        signature += name + "->" + return_type->to_string() + "(";

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

} // namespace Core
