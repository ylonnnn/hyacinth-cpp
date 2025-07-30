#include "ast/expr/FunctionCallExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionCalLExpr::FunctionCalLExpr(
        std::unique_ptr<Expr> callee,
        std::vector<std::unique_ptr<Expr>> arguments)
        : Node(callee->position()), callee_(std::move(callee)),
          arguments_(std::move(arguments))
    {
        set_end_position(arguments_.empty()
                             ? callee_->end_position()
                             : arguments_.back()->end_position());
    }

    Expr &FunctionCalLExpr::callee() { return *callee_; }

    std::vector<std::unique_ptr<Expr>> &FunctionCalLExpr::arguments()
    {
        return arguments_;
    }

    void FunctionCalLExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "FunctionCallExpr {\n" << inner_indentation << "callee: ";
        callee_->print(os, tab + 1);

        os << "\n" << inner_indentation << "arguments: {";
        for (auto &argument : arguments_)
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);
            os << "\n" << inner_indentation;
            argument->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
