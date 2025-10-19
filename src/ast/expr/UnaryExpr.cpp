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
        end_position = unary_type == UnaryType::Pre ? this->expr->end_position
                                                    : &operation.range.end;
    }

    void UnaryExpr::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "UnaryExpr {\n"
           << inner_indentation << "operation: " << operation.value << "\n"
           << inner_indentation << "expr: ";

        expr->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
