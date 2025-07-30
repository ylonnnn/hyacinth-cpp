#include "core/symbol/VariableSymbol.hpp"
#include "core/symbol/IdentifierSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    VariableSymbol::VariableSymbol(std::string_view name,
                                   Core::Position declared_at, bool is_mutable,
                                   std::unique_ptr<Type> type,
                                   std::optional<Value> value,
                                   AST::VariableDeclarationStmt *node)
        : IdentifierSymbol(name, declared_at, is_mutable, std::move(type),
                           std::move(value), node)
    {
        this->node = dynamic_cast<AST::VariableDeclarationStmt *>(Symbol::node);
    }

} // namespace Core
