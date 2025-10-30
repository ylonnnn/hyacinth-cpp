#include "core/symbol/IdentifierSymbol.hpp"
#include "core/symbol/Symbol.hpp"
#include "utils/style.hpp"

namespace Core
{
    IdentifierSymbol::IdentifierSymbol(std::string_view name,
                                       AST::Node *decl_node, bool is_mutable,
                                       InstantiatedType *type, Value *value)
        : Symbol(name, SymbolAccessibility::Private, decl_node),
          is_mutable(is_mutable), type(type), value(value)
    {
    }

    void IdentifierSymbol::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << name << " { "
           << (accessibility == SymbolAccessibility::Public ? "Public"
                                                            : "Private")
           << ", " << (is_mutable ? "Mutable" : "Immutable") << ", "
           << (type == nullptr ? "nullptr" : type->to_string()) << ", "
           << (value != nullptr ? value->to_string() : "null") << " }";
    }

} // namespace Core
