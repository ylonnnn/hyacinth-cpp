#pragma once

#include <vector>

#include "ast/stmt/Stmt.hpp"

namespace AST
{
    struct BlockStmt : virtual Stmt
    {
        std::vector<std::unique_ptr<Stmt>> statements;

        BlockStmt(Core::Position &position,
                  std::vector<std::unique_ptr<Stmt>> &&statements);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
