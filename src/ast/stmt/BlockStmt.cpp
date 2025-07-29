#include "ast/stmt/BlockStmt.hpp"

namespace AST
{
    BlockStmt::BlockStmt(Core::Position position,
                         std::vector<std::unique_ptr<Stmt>> statements)
        : Node(std::move(position)), statements_(std::move(statements))
    {
    }

    size_t BlockStmt::end_pos() const { return end_pos_; }

    void BlockStmt::set_end_pos(size_t end_pos) { end_pos_ = end_pos; }

    std::vector<std::unique_ptr<Stmt>> &BlockStmt::statements()
    {
        return statements_;
    }

    void BlockStmt::print([[maybe_unused]] std::ostream &os,
                          [[maybe_unused]] uint8_t tab) const
    {
    }

} // namespace AST
