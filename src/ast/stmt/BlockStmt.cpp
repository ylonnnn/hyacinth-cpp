#include "ast/stmt/BlockStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    BlockStmt::BlockStmt(std::unique_ptr<Block> block)
        : Node(block->position()), block_(std::move(block))
    {
    }

    Block &BlockStmt::block() { return *block_; }

    const Block &BlockStmt::block() const { return *block_; }

    std::unique_ptr<Block> &BlockStmt::block_ptr() { return block_; }

    const std::unique_ptr<Block> &BlockStmt::block_ptr() const
    {
        return block_;
    }

    void BlockStmt::print(std::ostream &os, uint8_t tab) const
    {
        os << "BlockStmt {\n" << utils::tab(tab, 4) << "block: ";

        block_->print(os, tab + 1);

        os << "\n" << utils::tab(tab - 1, 4) << "}";
    }

} // namespace AST
