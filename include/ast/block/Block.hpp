#pragma once

#include <vector>

#include "ast/Node.hpp"
#include "ast/stmt/Stmt.hpp"

namespace AST
{
    struct Block : virtual Node
    {
        std::vector<std::unique_ptr<Stmt>> statements;

        Block(Core::Position &position,
              std::vector<std::unique_ptr<Stmt>> &&statements);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
