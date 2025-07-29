#include "core/symbol/type/StructSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    StructSymbol::StructSymbol(std::string name, Core::Position declared_at,
                               std::vector<StructField> &&fields,
                               AST::StructDeclarationStmt *node)
        : Symbol(name, std::move(declared_at), node)
    {
        this->node = dynamic_cast<AST::StructDeclarationStmt *>(Symbol::node);

        this->fields.reserve(fields.size());
        for (auto &[name, field] : fields)
            this->fields.try_emplace(name, std::move(field));
    }

} // namespace Core
