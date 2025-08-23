#pragma once

#include <vector>

#include "ast/Node.hpp"
#include "ast/stmt/Stmt.hpp"

namespace AST
{
    class Block : virtual public Node
    {
      protected:
        std::vector<std::unique_ptr<Stmt>> statements_;

      public:
        Block(Core::Position &position,
              std::vector<std::unique_ptr<Stmt>> &&statements);

        std::vector<std::unique_ptr<Stmt>> &statements();
        const std::vector<std::unique_ptr<Stmt>> &statements() const;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
