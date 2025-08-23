#include "ast/block/LibBlock.hpp"
#include "utils/style.hpp"

namespace AST
{
    LibBlock::LibBlock(Lexer::Token &name)
        : Node(name.position),
          GlobalBlock(name.position, std::vector<std::unique_ptr<Stmt>>{}),
          name_(name)
    {
    }

    Lexer::Token &LibBlock::name() { return name_; }

    const Lexer::Token &LibBlock::name() const { return name_; }

    void LibBlock::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "LibBlock {\n"
           << inner_indentation << "name: " << name_ << "\n"
           << inner_indentation << "statements: {";

        for (const auto &statement : statements_)
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            statement->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
