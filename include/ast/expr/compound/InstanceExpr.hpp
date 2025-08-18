#pragma once

#include <unordered_map>

#include "ast/expr/Expr.hpp"
#include "ast/type/Type.hpp"

namespace AST
{
    class InstanceField : public Node
    {
      private:
        Lexer::Token &name_;
        std::unique_ptr<Expr> value_;

      public:
        InstanceField(Lexer::Token &name, std::unique_ptr<Expr> value);

        Lexer::Token &name();
        Expr &value();

        std::unique_ptr<Expr> &value_ptr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

    class InstanceExpr : public Expr
    {
      private:
        std::unique_ptr<Type> type_;
        std::unordered_map<std::string, InstanceField> fields_;

      public:
        InstanceExpr(std::unique_ptr<Type> type,
                     std::unordered_map<std::string, InstanceField> fields);
        InstanceExpr(Core::Position &position,
                     std::unordered_map<std::string, InstanceField> fields);

        Type &type();
        std::unordered_map<std::string, InstanceField> &fields();

        std::unique_ptr<Type> &type_ptr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
