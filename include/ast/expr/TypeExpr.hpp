#pragma once

#include "ast/expr/IdentifierExpr.hpp"
#include "ast/type/Type.hpp"

namespace AST
{
    class TypeExpr : public IdentifierExpr
    {
      private:
        std::unique_ptr<Type> type_;

      public:
        TypeExpr(std::unique_ptr<Type> type);

        Type &type();
        std::unique_ptr<Type> &type_ptr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
