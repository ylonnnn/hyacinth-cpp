#include "ast/stmt/DeclarationStmt.hpp"

namespace AST
{
    std::ostream &operator<<(std::ostream &os,
                             DeclarationAccessibility accessibility)
    {
        return os << (accessibility == DeclarationAccessibility::Public
                          ? "Public"
                          : "Private");
    }

    DeclarationStmt::DeclarationStmt(Lexer::Token &name,
                                     DeclarationAccessibility accessibility,
                                     bool is_definition)
        : Node(name.range.start), name(name), accessibility(accessibility),
          is_definition_(is_definition)
    {
    }

    bool DeclarationStmt::is_definition() const { return is_definition_; }

} // namespace AST
