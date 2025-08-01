#pragma once

#include "ast/stmt/Stmt.hpp"

namespace AST
{
    class GlobalStmt : public Stmt
    {
      public:
        GlobalStmt() = default;
        virtual ~GlobalStmt() = default;

        virtual void print(std::ostream &os, uint8_t tab) const override = 0;
    };

} // namespace AST
