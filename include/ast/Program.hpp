#pragma once

#include <memory>
#include <vector>

#include "ast/Node.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    class Program : public Node
    {
      private:
        Core::ProgramFile &program_;
        std::vector<std::unique_ptr<DeclarationStmt>> declarations_;

      public:
        Program(Core::ProgramFile &program);

        std::vector<std::unique_ptr<DeclarationStmt>> &declarations();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
