#pragma once

#include <memory>
#include <vector>

#include "ast/Node.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "core/position/Position.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    // Mediator
    struct ProgramNode
    {
        ProgramNode(Core::Program &program);
        ~ProgramNode() = default;

      protected:
        Core::Position position_;
    };

    struct Program : ProgramNode, Node
    {
        Core::Program &program;
        std::vector<std::unique_ptr<DeclarationStmt>> nodes;

        Program(Core::Program &program);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
