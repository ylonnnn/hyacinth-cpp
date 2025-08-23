#include "ast/block/GlobalBlock.hpp"

namespace AST
{
    GlobalBlock::GlobalBlock(
        Core::Position &position,
        std::vector<std::unique_ptr<AST::Stmt>> &&statements)
        : Node(position), Block(position, std::move(statements))
    {
    }

} // namespace AST
