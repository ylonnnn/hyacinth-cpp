#include "ast/Program.hpp"
#include "ast/Node.hpp"
#include "core/program/Program.hpp"
#include "utils/style.hpp"

namespace AST
{
    ProgramNode::ProgramNode(Core::Program &program)
        : position_(program.position_at(1, 1, 0))
    {
    }

    Program::Program(Core::Program &program)
        : ProgramNode(program), Node(ProgramNode::position_), program(program),
          nodes()
    {
        nodes.reserve(64);
    }

    void Program::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "Program {";

        for (auto &declaration : nodes)
        {
            os << "\n" << inner_indentation;
            declaration->print(os, tab + 1);
        }

        os << "\n" << indentation << "}";
    }

} // namespace AST
