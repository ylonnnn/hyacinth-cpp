#pragma once

#include <memory>
#include <vector>

#include "ast/Node.hpp"
#include "ast/stmt/GlobalStmt.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    // Mediator
    class ProgramNode
    {
      protected:
        Core::Position position_;

      public:
        ProgramNode(Core::ProgramFile &program);
        ~ProgramNode() = default;
    };

    class Program : public ProgramNode, public Node
    {
      private:
        Core::ProgramFile &program_;
        std::vector<std::unique_ptr<GlobalStmt>> statements_;

      public:
        Program(Core::ProgramFile &program);

        std::vector<std::unique_ptr<GlobalStmt>> &statements();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
