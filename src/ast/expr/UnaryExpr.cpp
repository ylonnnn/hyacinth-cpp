#include <iostream>

#include "ast/expr/Expr.hpp"
#include "ast/expr/UnaryExpr.hpp"
#include "lexer/Token.hpp"
#include "utils/style.hpp"

namespace AST
{
    UnaryExpr::UnaryExpr(UnaryType unary_type, Lexer::Token &operation,
                         std::unique_ptr<Expr> &&expr)
        : Node(unary_type == UnaryType::Pre ? operation.range.start
                                            : expr->position),
          unary_type(unary_type), operation(operation), expr(std::move(expr))
    {
        end_position = unary_type == UnaryType::Pre ? expr->end_position
                                                    : &operation.range.end;
    }

    void UnaryExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    tabs = utils::tab(tab, 4);

        os << "UnaryExpr {"
           << "\n"
           << tabs << "operation: " << operation.value << "\n"
           << tabs << "expr: ";

        expr->print(os, tab + 4);

        os << "\n" << indentation << "}";
    }

} // namespace AST
