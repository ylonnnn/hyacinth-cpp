#include "ast/Program.hpp"
#include "ast/Node.hpp"
#include "core/program/Program.hpp"
#include "utils/style.hpp"

namespace AST
{
    ProgramNode::ProgramNode(Core::ProgramFile &program)
        : position_(program.position_at(1, 1))
    {
    }

    Program::Program(Core::ProgramFile &program)
        : ProgramNode(program), Node(ProgramNode::position_), program_(program),
          statements_()
    {
        statements_.reserve(64);
    }

    std::vector<std::unique_ptr<GlobalStmt>> &Program::statements()
    {
        return statements_;
    }

    void Program::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "Program {";

        for (auto &declaration : statements_)
        {
            std::cout << "\n" << inner_indentation;
            declaration->print(os, tab + 1);
        }

        os << "\n" << indentation << "}";
    }

} // namespace AST
