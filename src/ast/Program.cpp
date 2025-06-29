#include "ast/Program.hpp"
#include "ast/Node.hpp"
#include "utils/style.hpp"

namespace AST
{
    Program::Program(::Program::ProgramFile &program)
        : Node(program.position_at(1, 1)), program_(program)
    {
        declarations_.reserve(64);
    }

    std::vector<std::unique_ptr<DeclarationStmt>> &Program::declarations()
    {
        return declarations_;
    }

    void Program::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);
        os << "Program {";

        for (auto &declaration : declarations_)
            declaration->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
