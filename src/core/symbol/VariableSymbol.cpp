#include "core/symbol/VariableSymbol.hpp"
#include "core/symbol/IdentifierSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    VariableSymbol::VariableSymbol(std::string_view name,
                                   SymbolAccessibility accessibility,
                                   Core::Position declared_at, bool is_mutable,
                                   Type *type, std::shared_ptr<Value> value,
                                   AST::VariableDeclarationStmt *node)
        : IdentifierSymbol(name, declared_at, is_mutable, std::move(type),
                           std::move(value), node)
    {
        this->accessibility = accessibility;
        this->node = dynamic_cast<AST::VariableDeclarationStmt *>(Symbol::node);
    }

    void VariableSymbol::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "(var) " << name << " { "
           << (accessibility == SymbolAccessibility::Public ? "Public"
                                                            : "Private")
           << ", " << (is_mutable ? "Mutable" : "Immutable") << ", "
           << type->to_string() << ", " << (value != nullptr ? *value : "null")
           << " }";
    }

} // namespace Core
