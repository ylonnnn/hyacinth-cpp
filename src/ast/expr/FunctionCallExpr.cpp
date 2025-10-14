#include "ast/expr/FunctionCallExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionCallExpr::FunctionCallExpr(
        std::unique_ptr<Expr> callee,
        std::vector<std::unique_ptr<Expr>> arguments)
        : Node(callee->position), callee(std::move(callee)),
          arguments(std::move(arguments))
    {
        end_position = this->arguments.empty()
                           ? this->callee->end_position
                           : this->arguments.back()->end_position;
    }

    // void FunctionCallExpr::set_fn_symbol(Core::FunctionSymbol *fn_symbol)
    // {
    //     fn_symbol_ = fn_symbol;
    // }

    void FunctionCallExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionCallExpr {\n" << inner_indentation << "callee: ";
        callee->print(os, tab + 1);

        os << "\n" << inner_indentation << "arguments: {";
        for (auto &argument : arguments)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            argument->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
