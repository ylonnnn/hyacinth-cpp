#include "ast/stmt/function/FunctionRetStmt.hpp"
#include "ast/Node.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionReturnStmt::FunctionReturnStmt(Core::Position &position,
                                           std::unique_ptr<Expr> value)
        : Node(position), return_value_(std::move(value))
    {
        if (value_ != nullptr)
            set_end_position(return_value_->end_position());
    }

    Expr *FunctionReturnStmt::return_value() { return return_value_.get(); }

    std::unique_ptr<Expr> &FunctionReturnStmt::return_value_ptr()
    {
        return return_value_;
    }

    void FunctionReturnStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "FunctionReturnStmt {\n" << inner_indentation << "value: ";

        if (return_value_ != nullptr)
            return_value_->print(os, tab + 1);
        else
            os << "null";

        os << "\n" << indentation << "}";
    }

} // namespace AST
