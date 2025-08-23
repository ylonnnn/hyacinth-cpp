#pragma once

#include "ast/block/Block.hpp"
#include "ast/stmt/Stmt.hpp"

namespace AST
{
    class BlockStmt : public Stmt
    {
      private:
        std::unique_ptr<Block> block_;

      public:
        BlockStmt(std::unique_ptr<Block> block);

        Block &block();
        const Block &block() const;

        std::unique_ptr<Block> &block_ptr();
        const std::unique_ptr<Block> &block_ptr() const;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
