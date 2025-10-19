#include "ast/stmt/BlockStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    BlockStmt::BlockStmt(Core::Position &position,
                         std::vector<std::unique_ptr<Stmt>> &&statements)
        : Node(position), statements(std::move(statements))
    {
    }

    void BlockStmt::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "Block {\n" << inner_indentation << "statements: {";

        for (const auto &statement : statements)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            statement->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
