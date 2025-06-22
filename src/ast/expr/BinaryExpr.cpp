#include <iostream>

#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"
#include "utils/style.hpp"

namespace AST
{
    BinaryExpr::BinaryExpr(Expr &left, Lexer::Token &operation, Expr &right)
        : Expr(Program::Position(left.position())), left_(left),
          operation_(operation), right_(right)
    {
         end_pos_ = right_.end_pos();
    }

    Expr &BinaryExpr::left() { return left_; }

    Lexer::Token &BinaryExpr::operation() { return operation_; }

    Expr &BinaryExpr::right() { return right_; }

    void BinaryExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string less_indented = Utils::tab(tab - 1), tabs = Utils::tab(tab);
        os << less_indented << "BinaryExpr {";

        os << "\n" << tabs << "left: ";
        left_.print(os, tab);

        os << "\n" << tabs << "operation: " << operation_.value;

        os << "\n" << tabs << "right: ";
        right_.print(os, tab);

        os << "\n" << less_indented << "}";
    }

} // namespace AST
