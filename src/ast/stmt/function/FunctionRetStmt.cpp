#include "ast/stmt/function/FunctionRetStmt.hpp"
#include "ast/Node.hpp"
#include "utils/dev.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionReturnStmt::FunctionReturnStmt(Core::Position &position,
                                           std::unique_ptr<Expr> value)
        : Node(position), return_value(std::move(value))
    {
        utils::todo("implement function return value");
        // if (value_ != nullptr)
        //     set_end_position(return_value_->end_position());
    }

    void FunctionReturnStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionReturnStmt {\n" << inner_indentation << "value: ";

        if (return_value != nullptr)
            return_value->print(os, tab + 1);
        else
            os << "null";

        os << "\n" << indentation << "}";
    }

} // namespace AST
