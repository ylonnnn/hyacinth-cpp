#include "core/symbol/VariableSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    VariableSymbol::VariableSymbol(std::string name, Core::Position declared_at,
                                   bool is_mutable, std::unique_ptr<Type> type,
                                   std::optional<Value> value,
                                   AST::VariableDeclarationStmt *node)
        : Symbol(std::move(name), std::move(declared_at), node),
          is_mutable(is_mutable), type(std::move(type)),
          value(value ? *value : nullptr)
    {
        this->node = dynamic_cast<AST::VariableDeclarationStmt *>(Symbol::node);
    }

} // namespace Core
