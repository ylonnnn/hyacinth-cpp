#include "ast/type/ConstantType.hpp"

namespace AST
{
    ConstantType::ConstantType(Lexer::Token &constant)
        : Type(constant), constant_(constant)
    {
    }

    Lexer::Token &ConstantType::constant() { return constant_; }

    std::string ConstantType::to_string() const
    {
        return std::string(value_.value);
    }

    void ConstantType::print(std::ostream &os, uint8_t) const
    {
        os << "ConstantType { constant: " << constant_ << " }";
    }

} // namespace AST
