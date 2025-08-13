#include <iostream>
#include <unordered_map>

#include "ast/expr/compound/InstanceExpr.hpp"
#include "utils/style.hpp"

namespace AST
{
    InstanceField::InstanceField(Lexer::Token &name,
                                 std::unique_ptr<Expr> value)
        : Node(name.position), name_(name), value_(std::move(value))
    {
        if (value_ != nullptr)
            set_end_position(value_->end_position());
    }

    Lexer::Token &InstanceField::name() { return name_; }

    Expr &InstanceField::value() { return *value_; }

    std::unique_ptr<Expr> &InstanceField::value_ptr() { return value_; }

    void InstanceField::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4);

        os << name_.value << ": ";
        value_->print(os, tab + 1);
    }

    InstanceExpr::InstanceExpr(
        std::unique_ptr<Type> type,
        std::unordered_map<std::string, InstanceField> fields)
        : Node(type->position()), type_(std::move(type)),
          fields_(std::move(fields))
    {
    }

    InstanceExpr::InstanceExpr(
        Core::Position &position,
        std::unordered_map<std::string, InstanceField> fields)
        : Node(position), type_(nullptr), fields_(std::move(fields))
    {
    }

    Type *InstanceExpr::type() { return type_.get(); }

    std::unordered_map<std::string, InstanceField> &InstanceExpr::fields()
    {
        return fields_;
    }

    std::unique_ptr<Type> &InstanceExpr::type_ptr() { return type_; }

    void InstanceExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "InstanceExpr {";

        os << "\n" << inner_indentation << "type: ";

        if (type_ != nullptr)
            type_->print(os, tab + 1);
        else
            os << "nullptr";

        for (const auto &[name, field] : fields_)
        {
            os << "\n" << inner_indentation;
            field.print(os, tab + 1);
        }

        os << "\n" << indentation << "}";
    }

} // namespace AST
