#include "ast/stmt/ImportStmt.hpp"
#include "ast/Node.hpp"

namespace AST
{
    ImportStmt::ImportStmt(Lexer::Token &target)
        : Node(target.position), target_(target)
    {
    }

    Lexer::Token &ImportStmt::target() { return target_; }

    std::string_view ImportStmt::target_strv() const { return target_.value; }

    void ImportStmt::print(std::ostream &os, uint8_t tab) const
    {
        os << "ImportStmt { target: " << target_ << " }";
    }

} // namespace AST
