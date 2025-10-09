#include <iostream>

#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"
#include "utils/style.hpp"

namespace AST
{
    BinaryExpr::BinaryExpr(std::unique_ptr<Expr> &&left,
                           Lexer::Token &operation,
                           std::unique_ptr<Expr> &&right)
        : Node(left->position), left(std::move(left)), operation(operation),
          right(std::move(right))
    {
        end_position =
            right != nullptr ? right->end_position : &operation.range.end;
    }

    void BinaryExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "BinaryExpr {"
           << "\n"
           << inner_indentation << "left: ";

        left->print(os, tab + 1);

        os << "\n"
           << inner_indentation << "operation: " << operation.value << "\n"
           << inner_indentation << "right: ";

        if (right != nullptr)
            right->print(os, tab + 1);
        else
            os << "nullptr";

        os << "\n" << indentation << "}";
    }

} // namespace AST
