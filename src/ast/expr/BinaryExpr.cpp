#include <iostream>

#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"
#include "utils/style.hpp"

namespace AST
{
    BinaryExpr::BinaryExpr(Lexer::Token &operation,
                           std::unique_ptr<Expr> &&left,
                           std::unique_ptr<Expr> &&right)
        : Node(left->range.start()), operation(operation),
          left(std::move(left)), right(std::move(right))
    {
        range.end(this->right != nullptr ? this->right->range.end()
                                         : operation.range.end());
    }

    void BinaryExpr::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "BinaryExpr {\n" << inner_indentation << "left: ";

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
