#include "ast/expr/StructLiteralExpr.hpp"

namespace AST
{
    StructLiteralExpr::StructLiteralExpr(Core::Position &position,
                                         std::unique_ptr<SimpleType> &&type)
        : Node(position), type(std::move(type))
    {
    }

    void StructLiteralExpr::print(std::ostream &os, uint32_t tab) const
    {
        // TODO: StructLiteralExpr::print()
    }

} // namespace AST
