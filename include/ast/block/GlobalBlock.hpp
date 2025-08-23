#pragma once

#include "ast/GlobalNode.hpp"
#include "ast/block/Block.hpp"

namespace AST
{
    class GlobalBlock : public GlobalNode, public Block
    {
      public:
        GlobalBlock(Core::Position &position,
                    std::vector<std::unique_ptr<AST::Stmt>> &&statements);

        virtual void print(std::ostream &os, uint8_t tab) const override = 0;
    };

} // namespace AST
