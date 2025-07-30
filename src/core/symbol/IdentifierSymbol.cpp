#include "core/symbol/IdentifierSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    IdentifierSymbol::IdentifierSymbol(
        std::string_view name, Core::Position declared_at, bool is_mutable,
        std::unique_ptr<Type> type, std::optional<Value> value, AST::Node *node)
        : Symbol(name, declared_at, node), is_mutable(is_mutable),
          type(std::move(type))
    {
        if (value)
            this->value = std::move(*value);
    }

} // namespace Core
