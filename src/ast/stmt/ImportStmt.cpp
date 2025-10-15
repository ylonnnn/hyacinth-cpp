#include "ast/stmt/ImportStmt.hpp"
#include "ast/Node.hpp"

namespace AST
{
    ImportStmt::ImportStmt(Lexer::Token &target)
        : Node(target.range.start),
          DeclarationStmt(target, DeclarationAccessibility::Private, true),
          target(target)
    {
    }

    void ImportStmt::print(std::ostream &os, [[maybe_unused]] uint8_t tab) const
    {
        os << "ImportStmt { target: " << target << " }";
    }

} // namespace AST
