#include "ast/stmt/function/FunctionRetStmt.hpp"
#include "ast/Node.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionReturnStmt::FunctionReturnStmt(Core::Position &position,
                                           std::unique_ptr<Expr> &&value)
        : Node(position), return_value(std::move(value))
    {
        if (return_value != nullptr)
            range.end(return_value->range.end());
    }

    void FunctionReturnStmt::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionReturnStmt {\n" << inner_indentation << "value: ";

        if (return_value != nullptr)
            return_value->print(os, tab + 1);
        else
            os << "nullptr";

        os << "\n" << indentation << "}";
    }

} // namespace AST
