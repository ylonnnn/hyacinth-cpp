#pragma once

#include <memory>
#include <vector>

#include "ast/Node.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "program/Program.hpp"

namespace AST
{
    class Program : public Node
    {
      private:
        ::Program::ProgramFile &program_;
        std::vector<std::unique_ptr<DeclarationStmt>> declarations_;

      public:
        Program(::Program::ProgramFile &program);

        std::vector<std::unique_ptr<DeclarationStmt>> &declarations();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
