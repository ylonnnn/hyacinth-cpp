#include "ast/expr/LiteralExpr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    LiteralExpr::LiteralExpr(Lexer::Token &value)
        : Node(value.range.start), value(value)
    {
        end_position = &value.range.end;
    }

    void LiteralExpr::print(std::ostream &os,
                            [[maybe_unused]] uint32_t tab) const
    {
        os << "LiteralExpr { value: " << value << " }";
    }

} // namespace AST
