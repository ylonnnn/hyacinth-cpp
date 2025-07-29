#include "ast/stmt/function/FunctionRetStmt.hpp"
#include "ast/Node.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionReturnStmt::FunctionReturnStmt(Core::Position position,
                                           std::unique_ptr<Expr> value)
        : Node(std::move(position)), value_(std::move(value))
    {
        if (value_ != nullptr)
            end_pos_ = value_->end_pos();
    }

    void FunctionReturnStmt::set_end_pos(size_t end_pos) { end_pos_ = end_pos; }

    Expr *FunctionReturnStmt::value() { return value_.get(); }

    std::unique_ptr<Expr> &FunctionReturnStmt::value_ptr() { return value_; }

    void FunctionReturnStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "FunctionReturnStmt {\n" << inner_indentation << "value: ";

        if (value_ != nullptr)
            value_->print(os, tab + 1);
        else
            os << "null";

        os << "\n" << indentation << "}";
    }

} // namespace AST
