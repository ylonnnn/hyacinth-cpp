#include "core/symbol/IdentifierSymbol.hpp"
#include "core/symbol/Symbol.hpp"
#include "utils/style.hpp"

namespace Core
{
    IdentifierSymbol::IdentifierSymbol(std::string_view name,
                                       Core::Position declared_at,
                                       bool is_mutable, Type *type,
                                       std::shared_ptr<Value> value,
                                       AST::Node *node)
        : Symbol(name, SymbolAccessibility::Private, declared_at, node),
          is_mutable(is_mutable), type(std::move(type)), value(std::move(value))
    {
    }

    void IdentifierSymbol::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << name << " { "
           << (accessibility == SymbolAccessibility::Public ? "Public"
                                                            : "Private")
           << ", " << (is_mutable ? "Mutable" : "Immutable") << ", "
           << type->to_string() << ", " << (value != nullptr ? *value : "null")
           << " }";
    }

} // namespace Core
