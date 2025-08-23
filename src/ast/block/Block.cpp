#include "ast/block/Block.hpp"
#include "utils/style.hpp"

namespace AST
{
    Block::Block(Core::Position &position,
                 std::vector<std::unique_ptr<Stmt>> &&statements)
        : Node(position), statements_(std::move(statements))
    {
    }

    std::vector<std::unique_ptr<Stmt>> &Block::statements()
    {
        return statements_;
    }

    const std::vector<std::unique_ptr<Stmt>> &Block::statements() const
    {
        return statements_;
    }

    void Block::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "Block {\n" << inner_indentation << "statements: {\n";

        for (const auto &statement : statements_)
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            statement->print(os, tab + 1);
        }

        os << inner_indentation << "}" << indentation << "}";
    }

} // namespace AST
