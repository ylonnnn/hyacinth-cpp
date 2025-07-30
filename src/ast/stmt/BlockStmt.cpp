#include "ast/stmt/BlockStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    BlockStmt::BlockStmt(Core::Position &position,
                         std::vector<std::unique_ptr<Stmt>> statements)
        : Node(position), statements_(std::move(statements))
    {
    }

    std::vector<std::unique_ptr<Stmt>> &BlockStmt::statements()
    {
        return statements_;
    }

    void BlockStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "BlockStmt {\n" << inner_indentation << "statements: {\n";

        for (const auto &statement : statements_)
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            statement->print(os, tab + 1);
        }

        os << inner_indentation << "}" << indentation << "}";
    }

} // namespace AST
