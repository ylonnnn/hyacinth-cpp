#include "ast/expr/compound/InstanceExpr.hpp"
#include "utils/style.hpp"
#include <iostream>
#include <unordered_map>

namespace AST
{
    InstanceExpr::InstanceExpr(
        Core::Position &position,
        std::unordered_map<std::string, std::unique_ptr<Expr>> fields)
        : Node(position), fields_(std::move(fields))
    {
    }

    std::unordered_map<std::string, std::unique_ptr<Expr>> &
    InstanceExpr::fields()
    {
        return fields_;
    }

    void InstanceExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "InstanceExpr {";

        for (const auto &[field, value] : fields_)
        {
            os << "\n" << inner_indentation << field << ": ";
            value->print(os, tab + 1);
        }

        os << "\n" << indentation << "}";
    }

} // namespace AST
