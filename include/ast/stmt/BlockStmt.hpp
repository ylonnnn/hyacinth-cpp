#pragma once

#include <vector>

#include "ast/stmt/Stmt.hpp"

namespace AST
{

    class BlockStmt : public Stmt
    {
      protected:
        std::vector<std::unique_ptr<Stmt>> statements_;

      public:
        BlockStmt(Core::Position position,
                  std::vector<std::unique_ptr<Stmt>> statements);

        size_t end_pos() const override;

        void set_end_pos(size_t end_pos);

        std::vector<std::unique_ptr<Stmt>> &statements();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
