#include "ast/expr/compound/InstanceExpr.hpp"
#include "utils/style.hpp"
#include <unordered_map>

namespace AST
{
    InstanceExpr::InstanceExpr(
        Core::Position position,
        std::unordered_map<std::string, std::unique_ptr<Expr>> fields)
        : Node(std::move(position)), fields_(std::move(fields))
    {
    }

    void InstanceExpr::set_end_pos(size_t end_pos) { end_pos_ = end_pos; }

    std::unordered_map<std::string, std::unique_ptr<Expr>> &
    InstanceExpr::fields()
    {
        return fields_;
    }

    void InstanceExpr::print(std::ostream &os, uint8_t tab) const
    {
        // TODO: InstanceExpr print()
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
